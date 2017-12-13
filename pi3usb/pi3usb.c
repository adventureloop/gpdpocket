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
#include <sys/clock.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/endian.h>
#include <sys/rman.h>
#include <sys/sysctl.h>

#include <machine/bus.h>
#include <machine/resource.h>

#include <dev/iicbus/iicbus.h>
#include <dev/iicbus/iiconf.h>

struct pi3usb_softc {
	device_t			sc_dev;
	uint8_t				sc_addr;
	uint8_t				sc_config;

};

#define PI3USB_SADDR	0x54 // datasheet disagrees with acpi

#define PI3USB_CFG_OPEN	0x00
#define PI3USB_CFG_4DPI	0x02
#define PI3USB_CFG_4DPIS	0x03
#define PI3USB_CFG_USB3	0x04
#define PI3USB_CFG_USB3S	0x05
#define PI3USB_CFG_USB32DPI	0x06
#define PI3USB_CFG_USB32DPIS	0x07

#define	CAST_PTR_INT(X) (*((int*)(X)))

static int pi3usb_probe(device_t);
static int pi3usb_attach(device_t);
static int pi3usb_detach(device_t);
static int pi3usb_read(device_t, uint8_t *);
static int pi3usb_write(device_t, uint8_t);

#if 0
static int
pi3usb_sysctl(SYSCTL_HANDLER_ARGS)
{
	struct pi3usb_softc *sc;

	sc = (struct pi3usb_softc *)arg1;

	if (req->newptr != NULL && CAST_PTR_INT(req->newptr) > 7)
		return (EINVAL);

	device_printf(sc->sc_dev, "debug req val %d\n", CAST_PTR_INT(req->newptr));
	return (sysctl_handle_int(oidp, arg1, arg2, req));
}
#endif

static int
pi3usb_probe(device_t dev)
{
	device_printf(dev, "probe\n");
	device_set_desc(dev, "pi3usb USB Type-C Mux");
	return (0);
}

static int
pi3usb_attach(device_t dev)
{
	struct pi3usb_softc *sc = device_get_softc(dev);
	int rv; 
	uint8_t config = 0;

	sc->sc_dev = dev;
	sc->sc_addr = PI3USB_SADDR << 1;

	if ((rv = pi3usb_read(dev, &config)) != 0)	
		device_printf(dev, "read config failed rv: %d errno: %d\n", rv, iic2errno(rv));
	else {
		device_printf(dev, "default config: %x\n", config);
		sc->sc_config = config;
	}

	return (0);
}

static int
pi3usb_detach(device_t dev)
{
	return (0);
}

static int 
pi3usb_read(device_t dev, uint8_t *val)
{
	struct pi3usb_softc *sc;
	struct iic_msg msg[1];
	uint8_t buf[2];
	int rv;

	sc = device_get_softc(dev);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_RD;
	msg[0].len = 2;
	msg[0].buf = buf;

	rv = iicbus_transfer(dev, msg, 1);
	*val = buf[1];

	return (rv);
	
}

static int 
pi3usb_write(device_t dev, uint8_t val)
{
	struct pi3usb_softc *sc;
	struct iic_msg msg[2];
	uint8_t buf[2];

	buf[0] = 0;
	buf[1] = val;

	sc = device_get_softc(dev);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR;
	msg[0].len = 2;
	msg[0].buf = buf;

	return (iicbus_transfer(dev, msg, 1));
}

static int
pi3usb_driver_loaded(struct module *m, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		uprintf("pi3usb KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("pi3usb KLD unloaded.\n");
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static device_method_t pi3usb_methods[] = {
	DEVMETHOD(device_probe,		pi3usb_probe),
	DEVMETHOD(device_attach,	pi3usb_attach),
	DEVMETHOD(device_detach,	pi3usb_detach),
	DEVMETHOD_END
};

static driver_t pi3usb_driver = {
	.name = "pi3usb",
	.methods = pi3usb_methods,
	.size = sizeof(struct pi3usb_softc)
};

static devclass_t pi3usb_devclass;
DRIVER_MODULE(pi3usb, iicbus, pi3usb_driver, pi3usb_devclass, pi3usb_driver_loaded, NULL);

MODULE_DEPEND(pi3usb, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(pi3usb, 1);
