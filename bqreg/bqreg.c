/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2018 Tom Jones <thj@freebsd.org>
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

struct bqreg_softc {
	device_t			sc_dev;
	uint8_t				sc_config;
};

#define BQREG_SADDR	0x54 // datasheet disagrees with acpi

#define BQREG_INPUT	0x00
#define BQREG_PWRCONF	0x01
#define BQREG_CHGCTRL	0x02
#define BQREG_PRECTRL	0x03
#define BQREG_VLTCTRL	0x04
#define BQREG_TMRCTRL	0x05
#define BQREG_IRCTRL	0x06
#define BQREG_MSCCTRL	0x07
#define BQREG_STATUS	0x08
#define BQREG_FAULT	0x09
#define BQREG_VENDER	0x0A

#define BQREG_STAT_VBUS		6	// 2 bits
#define BQREG_STAT_CHRG		4	// 2 bits
#define BQREG_STAT_DPM		3
#define BQREG_STAT_PG		2
#define BQREG_STAT_THERM	1
#define BQREG_STAT_VSYS		0

#define BQREG_VENDER_MASK	((x & 0x38) >> 3)
#define BQREG_VENDER_BQ24192I	3
#define BQREG_VENDER_BQ24190	4
#define BQREG_VENDER_BQ24192	5

static int bqreg_probe(device_t);
static int bqreg_attach(device_t);
static int bqreg_detach(device_t);
static int bqreg_read(device_t, uint8_t, uint8_t *);
static int bqreg_write(device_t, uint8_t, uint8_t);

static int
bqreg_probe(device_t dev)
{
	device_printf(dev, "probe\n");
	device_set_desc(dev, "bqreg power regulator");
	return (0);
}

static int
bqreg_attach(device_t dev)
{
	struct bqreg_softc *sc = device_get_softc(dev);
	int rv; 
	uint8_t config = 0;

	sc->sc_dev = dev;

	if ((rv = bqreg_read(dev, BQREG_VENDER, &config)) != 0)	
		device_printf(dev, "read config failed rv: %d errno: %d\n", rv, iic2errno(rv));
	else {
		device_printf(dev, "default config: %x\n", config);
		sc->sc_config = config;
	}

	return (0);
}

static int
bqreg_detach(device_t dev)
{
	return (0);
}

static int 
bqreg_read(device_t dev, uint8_t reg, uint8_t *val)
{
	struct bqreg_softc *sc;
	struct iic_msg msg[2];
	uint8_t data;
	int rv;
	uint16_t addr = iicbus_get_addr(dev); 

	sc = device_get_softc(dev);

	msg[0].slave = addr;
	msg[0].flags = IIC_M_WR;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = addr;
	msg[1].flags = IIC_M_RD;
	msg[1].len = 1;
	msg[1].buf = &data;

	rv = iicbus_transfer(dev, msg, 1);
	*val = data;

	return (rv);
}

static int 
bqreg_write(device_t dev, uint8_t reg, uint8_t val)
{
	struct bqreg_softc *sc;
	struct iic_msg msg[1];
	uint8_t buf[2];
	uint16_t addr = iicbus_get_addr(dev); 

	buf[0] = reg;
	buf[1] = val;

	sc = device_get_softc(dev);

	msg[0].slave = addr;
	msg[0].flags = IIC_M_WR;
	msg[0].len = 2;
	msg[0].buf = buf;

	return (iicbus_transfer(dev, msg, 1));
}

static device_method_t bqreg_methods[] = {
	DEVMETHOD(device_probe,		bqreg_probe),
	DEVMETHOD(device_attach,	bqreg_attach),
	DEVMETHOD(device_detach,	bqreg_detach),
	DEVMETHOD_END
};

static driver_t bqreg_driver = {
	.name = "bqreg",
	.methods = bqreg_methods,
	.size = sizeof(struct bqreg_softc)
};

static devclass_t bqreg_devclass;
DRIVER_MODULE(bqreg, iicbus, bqreg_driver, bqreg_devclass, NULL, NULL);

MODULE_DEPEND(bqreg, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(bqreg, 1);
