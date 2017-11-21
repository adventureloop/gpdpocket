/*-
 * Copyright (c) 2017 Tom Jones <tj@enoti.me>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
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

#include <machine/resource.h>

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>

#include "opt_platform.h"
#include "opt_acpi.h"

#define CHVPWM_CTRL	0
#define CHVPWM_RESET	0x804
#define CHVPWM_GENERAL	0x808

#define CHVPWM_CTRL_ENABLE	31
#define CHVPWM_CTRL_UPDATE	30
#define CHVPWM_CTRL_FREQ	((x) << 7)	//23:8 base unit register. 8 integer bits 8 fraction bits. used to determine PWM output frequency

#define CHVPWM_CTRL_DIV		0			//7:0

/*
 *     Macros for driver mutex locking
 */
#define CHVPWM_LOCK(_sc)               mtx_lock_spin(&(_sc)->sc_mtx)
#define CHVPWM_UNLOCK(_sc)             mtx_unlock_spin(&(_sc)->sc_mtx)
#define CHVPWM_LOCK_INIT(_sc) \
	mtx_init(&_sc->sc_mtx, device_get_nameunit((_sc)->sc_dev), \
	"chvpwm", MTX_SPIN)                      
#define CHVPWM_LOCK_DESTROY(_sc)       mtx_destroy(&(_sc)->sc_mtx)
#define CHVPWM_ASSERT_LOCKED(_sc)      mtx_assert(&(_sc)->sc_mtx, MA_OWNED)
#define CHVPWM_ASSERT_UNLOCKED(_sc) 	mtx_assert(&(_sc)->sc_mtx, MA_NOTOWNED)

struct chvpwm_softc {
	device_t 	sc_dev;
	device_t 	sc_busdev;
	struct mtx 	sc_mtx;

	ACPI_HANDLE	sc_handle;

	int		sc_mem_rid;
	struct resource *sc_mem_res;

	struct sysctl_oid *sc_clkdiv_oid;

	uint32_t	sc_pwm_period;
	uint32_t	sc_pwm_dutyA;
	uint32_t	sc_pwm_dutyB;
};

static int chvpwm_probe(device_t);
static int chvpwm_attach(device_t);
static int chvpwm_detach(device_t);

static inline int
chvpwm_read_ctrl(struct chvpwm_softc *sc)
{
	return bus_read_4(sc->sc_mem_res, CHVPWM_CTRL);
}

static inline void
chvpwm_write_ctrl(struct chvpwm_softc *sc, uint32_t val)
{
	bus_write_4(sc->sc_mem_res, CHVPWM_CTRL, val);
}
#if 0
static inline int
chvpwm_read_reset(struct chvpwm_softc *sc)
{
	return bus_read_4(sc->sc_mem_res, CHVPWM_CTRL_RESET);
}

static inline void
chvpwm_write_reset(struct chvpwm_softc *sc, uint32_t val)
{
	bus_write_4(sc->sc_mem_res, CHVPWM_CTRL_RESET, val);
}

static inline int
chvpwm_read_general(struct chvpwm_softc *sc)
{
	return bus_read_4(sc->sc_mem_res, CHVPWM_CTRL_GENERAL);
}

static inline void
chvpwm_write_general(struct chvpwm_softc *sc, uint32_t val)
{
	bus_write_4(sc->sc_mem_res, CHVPWM_CTRL_GENERAL, val);
}
#endif

static int
chvpwm_sysctl_freq(SYSCTL_HANDLER_ARGS)
{
     int error, freq;
     struct chvpwm_softc *sc;
//     uint32_t reg;

     sc = (struct chvpwm_softc *)arg1;

     error = sysctl_handle_int(oidp, &freq, sizeof(freq), req);
     if (error != 0 || req->newptr == NULL)
         return (error);

	device_printf(sc->sc_dev, "frequency %d Hz requested", freq);

     return (0);
}

static char *chvpwm_hids[] = {
	"80862288",
	"80862289",

"80860F09",
"80865AC8"
	NULL
};

static int
chvpwm_probe(device_t dev)
{
    if (acpi_disabled("chvpwm") ||
    ACPI_ID_PROBE(device_get_parent(dev), dev, chvpwm_hids) == NULL)
        return (ENXIO);

    device_set_desc(dev, "Intel Cherry View PWM");
    return (0);
}

static int
chvpwm_attach(device_t dev)
{
	struct chvpwm_softc *sc;
//	ACPI_STATUS status;
//	int uid;
//	struct sysctl_ctx_list *ctx;
//	struct sysctl_oid *tree;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;

	sc->sc_mem_rid = 0;
	sc->sc_mem_res = bus_alloc_resource_any(sc->sc_dev, SYS_RES_MEMORY, 
		&sc->sc_mem_rid, RF_ACTIVE);

	if (sc->sc_mem_res == NULL) {
		CHVPWM_LOCK_DESTROY(sc);
		device_printf(dev, "can't allocate memory resource\n");
		return (ENOMEM);
	}

	uint32_t regval;
	regval = chvpwm_read_ctrl(sc);

	device_printf(dev, "CTRL REG: %x\n", regval);

#if 0
	ctx = device_get_sysctl_ctx(sc->sc_dev);
	tree = device_get_sysctl_tree(sc->sc_dev);

	sc->sc_freq_oid = SYSCTL_ADD_PROC(ctx, SYSCTL_CHILDREN(tree), OID_AUTO,
		"freq", CTLTYPE_INT | CTLFLAG_RW, sc, 0,
		chvpwm_sysctl_freq, "I", "PWM frequency");
#endif

	return (0);
}

static int
chvpwm_detach(device_t dev)
{
	struct chvpwm_softc *sc;
	sc = device_get_softc(dev);

	if (sc->sc_mem_res != NULL)
		bus_release_resource(dev, SYS_RES_MEMORY, sc->sc_mem_rid, sc->sc_mem_res);

	CHVPWM_LOCK_DESTROY(sc);

    return (0);
}

static device_method_t chvpwm_methods[] = {
	DEVMETHOD(device_probe,     	chvpwm_probe),
	DEVMETHOD(device_attach,    	chvpwm_attach),
	DEVMETHOD(device_detach,    	chvpwm_detach),
	DEVMETHOD_END
};

static driver_t chvpwm_driver = {
    .name = "chvpwm",
    .methods = chvpwm_methods,
    .size = sizeof(struct chvpwm_softc)
};

static devclass_t chvpwm_devclass;
DRIVER_MODULE(chvpwm, acpi, chvpwm_driver, chvpwm_devclass, NULL , NULL);
MODULE_DEPEND(chvpwm, acpi, 1, 1, 1);

MODULE_VERSION(chvpwm, 1);
