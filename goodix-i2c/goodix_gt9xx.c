/*-
 * Copyright (c) 2017 Tom Jones tj@enoti.me
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
#include <sys/clock.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/endian.h>
#include <sys/rman.h>

#include <machine/bus.h>
#include <machine/resource.h>

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>

#include <dev/iicbus/iicbus.h>
#include <dev/iicbus/iiconf.h>

#include <dev/evdev/input.h>
#include <dev/evdev/evdev.h>

/* goodix GT9xx registers */

#define GOODIX_CMD	0x8040
#define GOODIX_ID	0x8140

struct goodix_softc {
	device_t			sc_dev;
	uint8_t				sc_addr;
	
	ACPI_HANDLE			sc_handle;


	struct resource 	*sc_irq_res;
	void				*sc_intrhand;

	int					sc_pen_down;
	int					sc_x;
	int					sc_y;
	struct evdev_dev 	*sc_evdev;
};

static char *goodix_hids[] = {
	"GDIX1001",
	NULL
};

static int goodix_detach(device_t);
static int goodix_read(device_t, uint16_t, uint8_t *, uint8_t);
static int goodix_write(device_t, uint16_t, uint8_t *, uint8_t);
static void goodix_intr(void *);
static void goodix_ev_report(struct goodix_softc *);

static ACPI_STATUS parse_resources(ACPI_RESOURCE *, void *); 

static int
goodix_attach(device_t dev)
{
	int res, rid, err;
	uint8_t buf[5];
	struct goodix_softc *sc;
	ACPI_STATUS status;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;

	sc->sc_handle = acpi_get_handle(dev);
	status = AcpiWalkResources(sc->sc_handle, "_CRS", parse_resources, dev);

	return (ENXIO);

	res = goodix_read(dev, GOODIX_CMD, buf, 4);
	if (res)
		device_printf(dev, "error I guess\n");

	buf[4] = '\0';	
	device_printf(dev, "goodix touch screen addr: %d, id %s\n", sc->sc_addr, buf);

	rid = 0;
	sc->sc_irq_res = bus_alloc_resource_any(dev, SYS_RES_IRQ, &rid, RF_ACTIVE);
	if (!sc->sc_irq_res) {
		device_printf(dev, "cannot allocate interrupt\n");
		return (ENXIO);
	}

	if (bus_setup_intr(dev, sc->sc_irq_res, INTR_TYPE_MISC | INTR_MPSAFE,
		NULL, goodix_intr, sc, &sc->sc_intrhand) != 0) {
		bus_release_resource(dev, SYS_RES_IRQ, 0, sc->sc_irq_res);
		device_printf(dev, "Unable to setup the irq handler.\n");
		return (ENXIO);
	}

	sc->sc_evdev = evdev_alloc();
	evdev_set_name(sc->sc_evdev, device_get_desc(dev));
	evdev_set_phys(sc->sc_evdev, device_get_nameunit(dev));
	evdev_set_id(sc->sc_evdev, BUS_VIRTUAL, 0, 0, 0);
	evdev_support_prop(sc->sc_evdev, INPUT_PROP_DIRECT);
	evdev_support_event(sc->sc_evdev, EV_SYN);
	evdev_support_event(sc->sc_evdev, EV_ABS);
	evdev_support_event(sc->sc_evdev, EV_KEY);

	//TODO fix these
	//evdev_support_abs(sc->sc_evdev, ABS_X, 0, 0,
		//ADC_MAX_VALUE, 0, 0, 0);
	//evdev_support_abs(sc->sc_evdev, ABS_Y, 0, 0,
		//ADC_MAX_VALUE, 0, 0, 0);

	evdev_support_key(sc->sc_evdev, BTN_TOUCH);

	err = evdev_register(sc->sc_evdev);
	if (err) {
		device_printf(dev,
			"failed to register evdev: error=%d\n", err);
		goodix_detach(dev);
		return (err);
	}

	sc->sc_pen_down = 0;
	sc->sc_x = -1;
	sc->sc_y = -1;

	return (0);
}

static int
goodix_detach(device_t dev)
{
	return (0);
}

static int 
goodix_read(device_t dev, uint16_t reg, uint8_t *data, uint8_t size)
{
	struct goodix_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);
	reg = htobe16(reg);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR;
	msg[0].len = 2;
	msg[0].buf = (uint8_t *)&reg;

	msg[1].slave = sc->sc_addr;
	msg[1].flags = IIC_M_RD;
	msg[1].len = size;
	msg[1].buf = data;

	return (iicbus_transfer(dev, msg, 2));
}

static int 
goodix_write(device_t dev, uint16_t reg, uint8_t *data, uint8_t size)
{
	struct goodix_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);
	reg = htobe16(reg);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 2;
	msg[0].buf = (uint8_t *)&reg; /* what is the byte order */

	/* this might not work as a transfer group */
	msg[1].slave = sc->sc_addr;		
	msg[1].flags = IIC_M_WR;
	msg[1].len = size;
	msg[1].buf = data;

	return (iicbus_transfer(dev, msg, 2));
}

static void
goodix_intr(void *arg)
{
	struct goodix_softc *sc;
	//uint32_t status, rawstatus;

	sc = (struct goodix_softc *)arg;

	//status = ADC_READ4(sc, ADC_IRQSTATUS);
	goodix_ev_report(sc);
}

static void
goodix_ev_report(struct goodix_softc *sc)
{

	evdev_push_event(sc->sc_evdev, EV_ABS, ABS_X, sc->sc_x);
	evdev_push_event(sc->sc_evdev, EV_ABS, ABS_Y, sc->sc_y);
	evdev_push_event(sc->sc_evdev, EV_KEY, BTN_TOUCH, sc->sc_pen_down);
	evdev_sync(sc->sc_evdev);
}

static ACPI_STATUS
parse_resources(ACPI_RESOURCE *res, void *context)
{
	//int type;          
	struct link_count_request *req;               
	device_t dev = (device_t)context;             
	struct chvpower_softc *sc;                    
	sc = device_get_softc(dev);                   

	req = (struct link_count_request *)context;   
	device_printf(dev, "resource of number: %x\n", res->Type);

	return (AE_OK);

}

static int
goodix_driver_loaded(struct module *m, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		uprintf("goodix touch screen KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("goodix touch screen KLD unloaded.\n");
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static int
goodix_acpi_probe(device_t dev)
{
	if (acpi_disabled("goodix") ||
		ACPI_ID_PROBE(device_get_parent(dev), dev, goodix_hids) == NULL)
		return (ENXIO);

	device_set_desc(dev, "Goodix GT9xx Capacitive TouchScreen");
	return (0);
}

static int
goodix_acpi_detach(device_t dev)
{
	return (0);
}

static int
goodix_acpi_attach(device_t dev)
{
	struct goodix_softc *sc;
	int error;

	sc = device_get_softc(dev);

	sc->sc_dev = dev;
	sc->sc_addr = 0xBA;

	error = goodix_attach(sc->sc_dev);
	if (error)
		goodix_acpi_detach(sc->sc_dev);

	return (0);
}

static device_method_t goodix_methods[] = {
	DEVMETHOD(device_probe,		goodix_acpi_probe),
	DEVMETHOD(device_attach,	goodix_acpi_attach),
	DEVMETHOD(device_detach,	goodix_acpi_detach),
	DEVMETHOD_END
};

static driver_t goodix_driver = {
	.name = "goodix",
	.methods = goodix_methods,
	.size = sizeof(struct goodix_softc)
};

static devclass_t goodix_devclass;
DRIVER_MODULE(goodix, acpi, goodix_driver, goodix_devclass, goodix_driver_loaded, NULL);

MODULE_DEPEND(goodix, acpi, 1, 1, 1);
MODULE_DEPEND(goodix, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_DEPEND(goodix, evdev, 1, 1, 1);
MODULE_VERSION(goodix, 1);
