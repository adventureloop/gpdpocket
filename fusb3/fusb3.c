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

#include <machine/bus.h>
#include <machine/resource.h>

#include <dev/iicbus/iicbus.h>
#include <dev/iicbus/iiconf.h>

struct fusb3_softc {
	device_t			sc_dev;
	uint8_t				sc_addr;
};

#define FUSB3_SADDR	0x22	//I think I know this

#define	FUSB3_VERSION	0x01

#define	FUSB3_STAT0A	0x3C
#define	FUSB3_STAT1B	0x3D

#define	FUSB3_STAT0	0x40
#define	FUSB3_STAT1	0x41

static int fusb3_probe(device_t);
static int fusb3_attach(device_t);
static int fusb3_detach(device_t);
static int fusb3_read(device_t, uint8_t, uint8_t *);
static int fusb3_write(device_t, uint8_t, uint8_t );

static int
fusb3_probe(device_t dev)
{
	device_printf(dev, "probe\n");
	device_set_desc(dev, "fusb3 USB Type-C Controller with PD");
	return (0);
}

static int
fusb3_attach(device_t dev)
{
	device_printf(dev, "attach\n");
	struct fusb3_softc *sc = device_get_softc(dev);
	int rv;

	sc->sc_dev = dev;
	sc->sc_addr = FUSB3_SADDR;

	uint8_t version;

	rv = fusb3_read(dev, FUSB3_VERSION, &version);
	if ( rv != 0) {
		device_printf(dev, "failed to read version code: %d %d\n", 
			rv, iic2errno(rv));
		return ENXIO;
	}

	return (0);
}

static int
fusb3_detach(device_t dev)
{
	return (0);
}

static int 
fusb3_read(device_t dev, uint8_t reg, uint8_t *val)
{
	struct fusb3_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = sc->sc_addr;
	msg[1].flags = IIC_M_RD;
	msg[1].len = sizeof(uint8_t);
	msg[1].buf = val;

	return (iicbus_transfer(dev, msg, 2));
}

static int 
fusb3_write(device_t dev, uint8_t reg, uint8_t val)
{
	struct fusb3_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);
	reg = htobe16(reg);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = sc->sc_addr;		
	msg[1].flags = IIC_M_WR;
	msg[1].len = sizeof(uint8_t);
	msg[1].buf = &val;

	return (iicbus_transfer(dev, msg, 2));
}

static int
fusb3_driver_loaded(struct module *m, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		uprintf("fusb3 KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("fusb3 KLD unloaded.\n");
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static device_method_t fusb3_methods[] = {
	DEVMETHOD(device_probe,		fusb3_probe),
	DEVMETHOD(device_attach,	fusb3_attach),
	DEVMETHOD(device_detach,	fusb3_detach),
	DEVMETHOD_END
};

static driver_t fusb3_driver = {
	.name = "fusb3",
	.methods = fusb3_methods,
	.size = sizeof(struct fusb3_softc)
};

static devclass_t fusb3_devclass;
DRIVER_MODULE(fusb3, iicbus, fusb3_driver, fusb3_devclass, fusb3_driver_loaded, NULL);

MODULE_DEPEND(fusb3, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(fusb3, 1);
