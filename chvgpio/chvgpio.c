/*
 * Copyright (c) 2016 Mark Kettenis
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bus.h>
#include <sys/gpio.h>
#include <sys/clock.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/endian.h>
#include <sys/rman.h>
#include <sys/types.h>
#include <sys/malloc.h>

#include <machine/bus.h>
#include <machine/resource.h>

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>
#include <dev/gpio/gpiobusvar.h>

#include "opt_platform.h"
#include "opt_acpi.h"
#include "gpio_if.h"

/**
 *	Macros for driver mutex locking
 */
#define	CHVGPIO_LOCK(_sc)		mtx_lock_spin(&(_sc)->sc_mtx)
#define	CHVGPIO_UNLOCK(_sc)		mtx_unlock_spin(&(_sc)->sc_mtx)
#define	CHVGPIO_LOCK_INIT(_sc)		\
	mtx_init(&_sc->sc_mtx, device_get_nameunit((_sc)->sc_dev), \
	    "chvgpio", MTX_SPIN)
#define	CHVGPIO_LOCK_DESTROY(_sc)	mtx_destroy(&(_sc)->sc_mtx)
#define	CHVGPIO_ASSERT_LOCKED(_sc)	mtx_assert(&(_sc)->sc_mtx, MA_OWNED)
#define	CHVGPIO_ASSERT_UNLOCKED(_sc) mtx_assert(&(_sc)->sc_mtx, MA_NOTOWNED)

/* Ignore function check, no info is available at the moment */
#define PADCONF_FUNC_ANY    -1

#define CHVGPIO_INTERRUPT_STATUS		0x0300
#define CHVGPIO_INTERRUPT_MASK			0x0380
#define CHVGPIO_PAD_CFG0			0x4400
#define CHVGPIO_PAD_CFG1			0x4404

#define CHVGPIO_PAD_CFG0_GPIORXSTATE		0x00000001
#define CHVGPIO_PAD_CFG0_GPIOTXSTATE		0x00000002
#define CHVGPIO_PAD_CFG0_INTSEL_MASK		0xf0000000
#define CHVGPIO_PAD_CFG0_INTSEL_SHIFT		28

#define CHVGPIO_PAD_CFG1_INTWAKECFG_MASK	0x00000007
#define CHVGPIO_PAD_CFG1_INTWAKECFG_FALLING	0x00000001
#define CHVGPIO_PAD_CFG1_INTWAKECFG_RISING	0x00000002
#define CHVGPIO_PAD_CFG1_INTWAKECFG_BOTH	0x00000003
#define CHVGPIO_PAD_CFG1_INTWAKECFG_LEVEL	0x00000004
#define CHVGPIO_PAD_CFG1_INVRXTX_MASK		0x000000f0
#define CHVGPIO_PAD_CFG1_INVRXTX_RXDATA		0x00000040

struct chvgpio_softc {
	device_t 	sc_dev;
	device_t 	sc_busdev;
	struct mtx 	sc_mtx;

	ACPI_HANDLE	sc_handle;

	int		sc_mem_rid;
	struct resource *sc_mem_res;

	int		sc_irq_rid;
	struct resource *sc_irq_res;

	void		*intr_handle;

	const char	*sc_bank_prefix;
	const int  	*sc_pins;
	int 		sc_npins;
	int 		sc_ngroups;
};

/*
 * The pads for the pins are arranged in groups of maximal 15 pins.
 * The arrays below give the number of pins per group, such that we
 * can validate the (untrusted) pin numbers from ACPI.
 */

#define	SW_UID		1
#define	SW_BANK_PREFIX	"southwestbank"

const int chv_southwest_pins[] = {
	8, 8, 8, 8, 8, 8, 8, -1
};

#define	SW_PIN_GROUPS	nitems(chv_southwest_pins)

#define	N_UID		2
#define	N_BANK_PREFIX	"northbank"

const int chv_north_pins[] = {
	9, 13, 12, 12, 13, -1
};

#define	N_PIN_GROUPS	nitems(chv_north_pins)

#define	E_UID		3
#define	E_BANK_PREFIX	"eastbank"

const int chv_east_pins[] = {
	12, 12, -1
};

#define	E_PIN_GROUPS	nitems(chv_east_pins)

#define	SE_UID		4
#define	SE_BANK_PREFIX	"southeastbank"

const int chv_southeast_pins[] = {
	8, 12, 6, 8, 10, 11, -1
};

#define	SE_PIN_GROUPS	nitems(chv_southeast_pins)

static void chvgpio_intr(void *);
static int chvgpio_probe(device_t);
static int chvgpio_attach(device_t);
static int chvgpio_detach(device_t);

static inline int
chvgpio_pad_cfg0_offset(int pin)
{
	return (CHVGPIO_PAD_CFG0 + 1024 * (pin / 15) + 8 * (pin % 15));
}

static inline int
chvgpio_read_pad_cfg0(struct chvgpio_softc *sc, int pin)
{
	return bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(pin));
}

static inline void
chvgpio_write_pad_cfg0(struct chvgpio_softc *sc, int pin, uint32_t val)
{
	bus_write_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(pin), val);
}

static inline int
chvgpio_read_pad_cfg1(struct chvgpio_softc *sc, int pin)
{
	return bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(pin) + 4);
}

static inline void
chvgpio_write_pad_cfg1(struct chvgpio_softc *sc, int pin, uint32_t val)
{
	bus_write_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(pin) + 4, val);
}

static device_t
chvgpio_get_bus(device_t dev)
{
	struct chvgpio_softc *sc;

	sc = device_get_softc(dev);

	return (sc->sc_busdev);
}

static int 
chvgpio_pin_max(device_t dev, int *maxpin)
{
	struct chvgpio_softc *sc;

	sc = device_get_softc(dev);

	*maxpin = sc->sc_npins - 1;

	return (0);
}

static int
chvgpio_valid_pin(struct chvgpio_softc *sc, int pin)
{
	if (pin < 0)
		return EINVAL;
	if ((pin / 15) >= sc->sc_ngroups)
		return EINVAL;
	if ((pin % 15) >= sc->sc_pins[pin / 15])
		return EINVAL;
	return (0);
}

static int 
chvgpio_pin_getname(device_t dev, uint32_t pin, char *name)
{
	struct chvgpio_softc *sc;

	sc = device_get_softc(dev);
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	/* Set a very simple name */
	snprintf(name, GPIOMAXNAME, "%s%u", sc->sc_bank_prefix, pin);
	name[GPIOMAXNAME - 1] = '\0';

	return (0);
}

#if 0
static int 
chvgpio_pin_getcaps(device_t dev, uint32_t pin, uint32_t *caps)
{
	struct chvgpio_softc *sc;

	sc = device_get_softc(dev);
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	*caps = 0;
	if (chvgpio_pad_is_gpio(sc, pin))
		*caps = GPIO_PIN_INPUT | GPIO_PIN_OUTPUT;

	return (0);
}

static int
chvgpio_pin_setflags(device_t dev, uint32_t pin, uint32_t flags)
{
	struct chvgpio_softc *sc;
	uint32_t reg, val;
	uint32_t allowed;

	sc = device_get_softc(dev);
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	if (chvgpio_pad_is_gpio(sc, pin))
		allowed = GPIO_PIN_INPUT | GPIO_PIN_OUTPUT;
	else
		allowed = 0;

	/* 
	 * Only directtion flag allowed
	 */
	if (flags & ~allowed)
		return (EINVAL);

	/* 
	 * Not both directions simultaneously
	 */
	if ((flags & allowed) == allowed)
		return (EINVAL);

	/* Set the GPIO mode and state */
	CHVGPIO_LOCK(sc);
	reg = CHVGPIO_PIN_REGISTER(sc, pin, CHVGPIO_PAD_VAL);
	val = chvgpio_read_4(sc, reg);
	val = val | CHVGPIO_PAD_VAL_DIR_MASK;
	if (flags & GPIO_PIN_INPUT)
		val = val & ~CHVGPIO_PAD_VAL_I_INPUT_ENABLED;
	if (flags & GPIO_PIN_OUTPUT)
		val = val & ~CHVGPIO_PAD_VAL_I_OUTPUT_ENABLED;
	chvgpio_write_4(sc, reg, val);
	CHVGPIO_UNLOCK(sc);

	return (0);
}
#endif

static int
chvgpio_pin_set(device_t dev, uint32_t pin, unsigned int value)
{
	struct chvgpio_softc *sc;
	uint32_t val;

	sc = device_get_softc(dev);
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	CHVGPIO_LOCK(sc);
	val = chvgpio_read_pad_cfg0(sc, pin);
	if (value == GPIO_PIN_LOW)
		val = val & ~CHVGPIO_PAD_CFG0_GPIOTXSTATE;
	else
		val = val | CHVGPIO_PAD_CFG0_GPIOTXSTATE;
	chvgpio_write_pad_cfg0(sc, pin, val);
	CHVGPIO_UNLOCK(sc);

	return (0);
}

static int
chvgpio_pin_get(device_t dev, uint32_t pin, unsigned int *value)
{
	struct chvgpio_softc *sc;
	uint32_t val;

	sc = device_get_softc(dev);
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	CHVGPIO_LOCK(sc);

	/*
	 * And read actual value
	 */
	val = chvgpio_read_pad_cfg0(sc, pin);
	if (val & CHVGPIO_PAD_CFG0_GPIORXSTATE)
		*value = GPIO_PIN_HIGH;
	else
		*value = GPIO_PIN_LOW;

	CHVGPIO_UNLOCK(sc);

	return (0);
}

static int
chvgpio_pin_toggle(device_t dev, uint32_t pin)
{
	struct chvgpio_softc *sc;
	uint32_t val;

	sc = device_get_softc(dev);
#if 0
	if (chvgpio_valid_pin(sc, pin) != 0)
		return (EINVAL);

	if (!chvgpio_pad_is_gpio(sc, pin))
		return (EINVAL);
#endif
	/* Toggle the pin */
	CHVGPIO_LOCK(sc);

	val = chvgpio_read_pad_cfg0(sc, pin);
	val = val ^ CHVGPIO_PAD_CFG0_GPIOTXSTATE;
	chvgpio_write_pad_cfg0(sc, pin, val);

	CHVGPIO_UNLOCK(sc);

	return (0);
}

static char *chvgpio_hids[] = {
	"INT33FF",
	NULL
};

static int
chvgpio_probe(device_t dev)
{
    if (acpi_disabled("chvgpio") ||
    ACPI_ID_PROBE(device_get_parent(dev), dev, chvgpio_hids) == NULL)
        return (ENXIO);

    device_set_desc(dev, "Intel Cherry View GPIO");
    return (0);
}

static int
chvgpio_attach(device_t dev)
{
	struct chvgpio_softc *sc;
	ACPI_STATUS status;
	int uid;
	int i;
	int error;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;
	sc->sc_handle = acpi_get_handle(dev);

	status = acpi_GetInteger(sc->sc_handle, "_UID", &uid);
	if (ACPI_FAILURE(status)) {
		device_printf(dev, "failed to read _UID\n");
		return (ENXIO);
	}

	CHVGPIO_LOCK_INIT(sc);

	switch (uid) {
	case SW_UID:
		sc->sc_npins = SW_PIN_GROUPS;
		sc->sc_bank_prefix = SW_BANK_PREFIX;
		sc->sc_pins = chv_southwest_pins;
		break;
	case N_UID:
		sc->sc_npins = N_PIN_GROUPS;
		sc->sc_bank_prefix = N_BANK_PREFIX;
		sc->sc_pins = chv_north_pins;
		break;
	case E_UID:
		sc->sc_npins = E_PIN_GROUPS;
		sc->sc_bank_prefix = E_BANK_PREFIX;
		sc->sc_pins = chv_east_pins;
		break;
	case SE_UID:
		sc->sc_npins = SE_PIN_GROUPS;
		sc->sc_bank_prefix = SE_BANK_PREFIX;
		sc->sc_pins = chv_southeast_pins;
		break;
	default:
		device_printf(dev, "invalid _UID value: %d\n", uid);
		return (ENXIO);
	}
	
	for (i = 0; sc->sc_pins[i] >= 0; i++) {
		sc->sc_npins += sc->sc_pins[i];
	}

	sc->sc_mem_rid = 0;
	sc->sc_mem_res = bus_alloc_resource_any(sc->sc_dev, SYS_RES_MEMORY, 
		&sc->sc_mem_rid, RF_ACTIVE);
	if (sc->sc_mem_res == NULL) {
		CHVGPIO_LOCK_DESTROY(sc);
		device_printf(dev, "can't allocate resource\n");
		return (ENOMEM);
	}

	sc->sc_irq_res = bus_alloc_resource_any(dev, SYS_RES_IRQ, 
		&sc->sc_irq_rid, RF_ACTIVE);

	if (!sc->sc_irq_res) {
		CHVGPIO_LOCK_DESTROY(sc);
		bus_release_resource(dev, SYS_RES_MEMORY, 
			sc->sc_mem_rid, sc->sc_mem_res);
		device_printf(dev, "IRQ allocation failed\n");
		return (ENOMEM);
	}

	error = bus_setup_intr(sc->sc_dev, sc->sc_irq_res, INTR_TYPE_MISC | INTR_MPSAFE,
		NULL, chvgpio_intr, sc, &sc->intr_handle);

	if (error) {
		device_printf(sc->sc_dev, "Unable to setup irq: error %d\n", error);
		CHVGPIO_LOCK_DESTROY(sc);
		bus_release_resource(dev, SYS_RES_MEMORY, 
			sc->sc_mem_rid, sc->sc_mem_res);
		bus_release_resource(dev, SYS_RES_IRQ, 
			sc->sc_irq_rid, sc->sc_irq_res);
		return (ENXIO);
	}

	/* Mask and ack all interrupts. */
	bus_write_4(sc->sc_mem_res, CHVGPIO_INTERRUPT_MASK, 0);
	bus_write_4(sc->sc_mem_res, CHVGPIO_INTERRUPT_STATUS, 0xffff);

#if 0
	// this magic that turns the fan on
	uint32_t value = 0;
	if (uid == N_UID) {
		value = bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(0));
		device_printf(dev, "read pin 0 location directly: 0x%x\n", value);

		device_printf(dev, "attempting to write to pin 0 location directly\n");
		value = value | CHVGPIO_PAD_CFG0_GPIOTXSTATE;
		bus_write_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(0), value);

		value = bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(0));
		device_printf(dev, "read pin 0 after directly writing: 0x%x\n", value);

		value = bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(1));
		device_printf(dev, "read pin 1 location directly: 0x%x\n", value);

		device_printf(dev, "attempting to write to pin 1 location directly\n");
		value = value | CHVGPIO_PAD_CFG0_GPIOTXSTATE;
		bus_write_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(1), value);

		value = bus_read_4(sc->sc_mem_res, chvgpio_pad_cfg0_offset(1));
		device_printf(dev, "read pin 1 after directly writing: 0x%x\n", value);
	}
#endif

	sc->sc_busdev = gpiobus_attach_bus(dev);
	if (sc->sc_busdev == NULL) {
		CHVGPIO_LOCK_DESTROY(sc);
		bus_release_resource(dev, SYS_RES_MEMORY, 
			sc->sc_mem_rid, sc->sc_mem_res);
		bus_release_resource(dev, SYS_RES_IRQ, 
			sc->sc_irq_rid, sc->sc_irq_res);
		return (ENXIO);
	}
	device_printf(sc->sc_busdev, "gpiobus_attach_bus returned device");

	return (0);
}

static void
chvgpio_intr(void *arg)
{
	struct chvgpio_softc *sc = arg;
	uint32_t reg;
	int line;

	reg = bus_read_4(sc->sc_mem_res,
	    CHVGPIO_INTERRUPT_STATUS);
	for (line = 0; line < 16; line++) {
		if ((reg & (1 << line)) == 0)
			continue;

		bus_write_4(sc->sc_mem_res,
		    CHVGPIO_INTERRUPT_STATUS, 1 << line);
/*
		if (sc->sc_pin_ih[line].ih_func)
			sc->sc_pin_ih[line].ih_func(sc->sc_pin_ih[line].ih_arg);
*/
	}
}

static int
chvgpio_detach(device_t dev)
{
    struct chvgpio_softc *sc;
    sc = device_get_softc(dev);
	
	if (sc->sc_busdev)
		gpiobus_detach_bus(dev);

	if ( sc->sc_irq_res != NULL)
		bus_release_resource(dev, SYS_RES_IRQ, sc->sc_irq_rid, sc->sc_irq_res);

	if ( sc->sc_mem_res != NULL)
		bus_release_resource(dev, SYS_RES_MEMORY, sc->sc_mem_rid, sc->sc_mem_res);

	CHVGPIO_LOCK_DESTROY(sc);

    return (0);
}

static int
chvgpio_driver_loaded(struct module *m, int what, void *arg)
{
    int err = 0;

    switch (what) {
    case MOD_LOAD:
        uprintf("chvgpio KLD loaded.\n");
        break;
    case MOD_UNLOAD:
        uprintf("chvgpio KLD unloaded.\n");
        break;
    default:
        err = EOPNOTSUPP;
        break;
    }
    return(err);
}

static device_method_t chvgpio_methods[] = {
    DEVMETHOD(device_probe,     chvgpio_probe),
    DEVMETHOD(device_attach,    chvgpio_attach),
    DEVMETHOD(device_detach,    chvgpio_detach),

	/* GPIO protocol */
	DEVMETHOD(gpio_get_bus, 	chvgpio_get_bus),
	DEVMETHOD(gpio_pin_max, 	chvgpio_pin_max),
	DEVMETHOD(gpio_pin_getname, 	chvgpio_pin_getname),
#if 0
	DEVMETHOD(gpio_pin_getflags,	chvgpio_pin_getflags),
	DEVMETHOD(gpio_pin_getcaps, 	chvgpio_pin_getcaps),
	DEVMETHOD(gpio_pin_setflags,	chvgpio_pin_setflags),
#endif
	DEVMETHOD(gpio_pin_get, 	chvgpio_pin_get),
	DEVMETHOD(gpio_pin_set, 	chvgpio_pin_set),
	DEVMETHOD(gpio_pin_toggle, 	chvgpio_pin_toggle),

    DEVMETHOD_END
};

static driver_t chvgpio_driver = {
    .name = "chvgpio",
    .methods = chvgpio_methods,
    .size = sizeof(struct chvgpio_softc)
};

static devclass_t chvgpio_devclass;
DRIVER_MODULE(chvgpio, acpi, chvgpio_driver, chvgpio_devclass, chvgpio_driver_loaded, NULL);
MODULE_DEPEND(chvgpio, acpi, 1, 1, 1);
MODULE_DEPEND(chvgpio, gpiobus, 1, 1, 1);

MODULE_VERSION(chvgpio, 1);
