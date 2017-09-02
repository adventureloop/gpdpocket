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

struct max170xx_softc {
	device_t			sc_dev;
	uint8_t				sc_addr;
};

#define MAX170xx_SADDR	0x36

#define	MAX170xx_STATUS	0x00
#define	MAX170xx_TEMP	0x08
#define MAX170xx_SOCAV	0x0E	//state of charge
#define	MAX170xx_TTE	0x11
#define	MAX170xx_CONFIG 0x1D
#define	MAX170xx_SOCVF	0xFF	//State Of Charge

static int max170xx_probe(device_t);
static int max170xx_attach(device_t);
static int max170xx_detach(device_t);
static int max170xx_read(device_t, uint8_t, uint16_t *);
static int max170xx_write(device_t, uint8_t, uint16_t );

static int
max170xx_probe(device_t dev)
{
	device_printf(dev, "probe\n");
	device_set_desc(dev, "Maxim max170xx Fuel Guage");
	return (0);
}

static int
max170xx_attach(device_t dev)
{
	device_printf(dev, "attach\n");
	struct max170xx_softc *sc = device_get_softc(dev);

	sc->sc_dev = dev;
	sc->sc_addr = MAX170xx_SADDR;

	uint16_t status = 0;
	uint16_t temp = 0;
	uint16_t tte = 0;
	uint16_t config = 0;
	uint16_t socvf = 0;
	uint16_t socav = 0;
	uint8_t remain = 0;

	max170xx_read(dev, MAX170xx_STATUS, &status);
	max170xx_read(dev, MAX170xx_TEMP, &temp);
	max170xx_read(dev, MAX170xx_SOCAV, &socav);
	max170xx_read(dev, MAX170xx_TTE, &tte);
	max170xx_read(dev, MAX170xx_CONFIG, &config);
	max170xx_read(dev, MAX170xx_SOCVF, &socvf);

	device_printf(dev, "fuel guage read: STATUS: %x TEMP: %x SOCKAV: %x"
		"TTE: %x CONFIG: %x SOCVF: %x\n", 
		status, temp, sockav, tte, config, socvf);

	remain = (socvf >> 8);
	remain = ((uint32_t)remain * 100) / 0xFF;

	device_printf(dev, "battery %d%%\n", remain);

	return (0);
}

static int
max170xx_detach(device_t dev)
{
	return (0);
}

static int 
max170xx_read(device_t dev, uint8_t reg, uint16_t *val)
{
	struct max170xx_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = sc->sc_addr;
	msg[1].flags = IIC_M_RD;
	msg[1].len = sizeof(uint16_t);
	msg[1].buf = (uint8_t *)val;

	return (iicbus_transfer(dev, msg, 2));
}

static int 
max170xx_write(device_t dev, uint8_t reg, uint16_t val)
{
	struct max170xx_softc *sc;
	struct iic_msg msg[2];

	sc = device_get_softc(dev);
	reg = htobe16(reg);

	msg[0].slave = sc->sc_addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = sc->sc_addr;		
	msg[1].flags = IIC_M_WR;
	msg[1].len = sizeof(uint16_t);
	msg[1].buf = (uint8_t *)&val;

	return (iicbus_transfer(dev, msg, 2));
}

static int
max170xx_driver_loaded(struct module *m, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		uprintf("max170xx fuel guage KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("max170xx fuel guage KLD unloaded.\n");
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static device_method_t max170xx_methods[] = {
	DEVMETHOD(device_probe,		max170xx_probe),
	DEVMETHOD(device_attach,	max170xx_attach),
	DEVMETHOD(device_detach,	max170xx_detach),
	DEVMETHOD_END
};

static driver_t max170xx_driver = {
	.name = "max170xx",
	.methods = max170xx_methods,
	.size = sizeof(struct max170xx_softc)
};

static devclass_t max170xx_devclass;
DRIVER_MODULE(max170xx, iicbus, max170xx_driver, max170xx_devclass, max170xx_driver_loaded, NULL);

MODULE_DEPEND(max170xx, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(max170xx, 1);
