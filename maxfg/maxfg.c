/*-
 * Copyright (c) 2018 Tom Jones <thj@freebsd.me>
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

#include <contrib/dev/acpica/include/acpi.h>

#include <dev/acpica/acpivar.h>
#include <dev/acpica/acpiio.h>

#define	MAXFG_REG_STATUS	0x00
#define MAXFG_REG_SALRT_TH	0x03	/* temperature alert */
#define	MAXFG_REG_TEMP		0x08	/* MSB +1C */
#define MAXFG_REG_VCELL		0x09	/* 0.625mV per div bottom 3 bits don't care */
#define MAXFG_REG_FULLCAP	0x10	/* calculated full cap in uVh */
#define MAXFG_REG_AVG_CUR	0x0B	/* average current */
#define MAXFG_REG_SOCAV		0x0E	/* state of charge */
#define	MAXFG_REG_TTE		0x11	/* time to empty */
#define MAXFG_REG_DESIGNCAP	0x18	/* design capacity in uVh */
#define MAXFG_REG_AVG_VOLT	0x19	/* average voltage */
#define	MAXFG_REG_CONFIG 	0x1D
#define	MAXFG_REG_REMCAP	0x1F	/* remaining capacity in uVh */
#define MAXFG_REG_VERSION	0x21
#define MAXFG_REG_VFOCV		0xFB	/* raw open-circuit voltage output */
#define	MAXFG_REG_SOCVF		0xFF	/* state of charge */

#define MAXFG_BIF_MODEL		"max17047/max17050"
#define MAXFG_BIF_SERIAL	"unknown"
#define MAXFG_BIF_TYPE		"fuel gauge"
#define MAXFG_BIF_OEMINFO	"unknown"

struct maxfg_softc {
	device_t	sc_dev;
	struct mtx	sc_mtx;

	uint32_t	sc_rsns;	/* sense resistor value in micro ohms */

	struct	acpi_bif sc_bif;
	struct	acpi_bst sc_bst;
};

static int maxfg_attach(device_t);
static int maxfg_detach(device_t);
static int maxfg_read(device_t, uint8_t, uint16_t *);
static void maxfg_dumpreg(device_t);

static int maxfg_get_bst(device_t, struct acpi_bst *);
static int maxfg_get_bif(device_t, struct acpi_bif *);

static int
maxfg_remaining(device_t dev)
{
	uint16_t socvf = 0;
	
	maxfg_read(dev, MAXFG_REG_SOCVF, &socvf);
	return (((socvf >> 8) * 100) + (((socvf & 0x00FF) * 100)/256) )/100;
}

void 
maxfg_dumpreg(device_t dev) 
{
	uint16_t reg = 0;

	maxfg_read(dev, MAXFG_REG_STATUS, &reg);
	device_printf(dev, "\tstatus %b\n", reg,
		"\10"
		"\001BER"
		"\002BEI"
		"\003AEN"
		"\004FTHRM"
		"\005ETHRM"
		"\006ALSH"
		"\007I2CSH"
		"\008SHDN"
		"\009TEX"
		"\010TEN"
		"\011AINSH"
		"\012ALRTp"
		"\013VS"
		"\014TS"
		"\015SS"
	);

	maxfg_read(dev, MAXFG_REG_CONFIG, &reg);
	device_printf(dev, "\tconfig %b\n", reg,
		"\10"
		"\002POR"
		"\004BST"
		"\009VMN"
		"\010TMN"
		"\011SMN"
		"\012BI"
		"\013VMX"
		"\014TMX"
		"\015SMX"
		"\016BR"
	);

	maxfg_read(dev, MAXFG_REG_SALRT_TH, &reg);
	device_printf(dev, "MAXFG_REG_SALRT_TH 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_TEMP, &reg);
	device_printf(dev, "MAXFG_REG_TEMP 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_VCELL, &reg);
	device_printf(dev, "MAXFG_REG_VCELL 0x%02x 0x%02x\n", reg, (reg>>3) * 1000/625);

	maxfg_read(dev, MAXFG_REG_FULLCAP, &reg);
	device_printf(dev, "MAXFG_REG_FULLCAP 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_SOCAV, &reg);
	device_printf(dev, "MAXFG_REG_SOCAV 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_TTE, &reg);
	device_printf(dev, "MAXFG_REG_TTE 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_DESIGNCAP, &reg);
	device_printf(dev, "MAXFG_REG_DESIGNCAP 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_REMCAP, &reg);
	device_printf(dev, "MAXFG_REG_REMCAP 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_VERSION, &reg);
	device_printf(dev, "MAXFG_REG_VERSION 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_VFOCV, &reg);
	device_printf(dev, "MAXFG_REG_VFOCV 0x%02x\n", reg);

	maxfg_read(dev, MAXFG_REG_SOCVF, &reg);
	device_printf(dev, "MAXFG_REG_SOCVF 0x%02x\n", reg);

	device_printf(dev, "battery %02d%%\n", maxfg_remaining(dev));	
}

static int
maxfg_probe(device_t dev)
{
	device_set_desc(dev, "Maxim max170xx Fuel Guage");
	return (0);
}

int
maxfg_attach(device_t dev)
{
	struct maxfg_softc *sc = device_get_softc(dev);
	int rv;
	uint16_t status, designcap, lastfullcap, designvolt;
	designcap = lastfullcap = designvolt = 0;

	sc->sc_dev = dev;

	/* datasheet recommends 0.01 ohms default sense resistor value */
	/* 0.01 ohms as microohms */
	sc->sc_rsns = 10;
	status = 0;

	rv = maxfg_read(sc->sc_dev, MAXFG_REG_STATUS, &status);
	if (rv != 0) {
		device_printf(sc->sc_dev, "failed to read status %d %d\n",
			rv, iic2errno(rv));
		return ENXIO;
	}

	//if (bootverbose)
	    maxfg_dumpreg(sc->sc_dev);

	rv = maxfg_read(sc->sc_dev, MAXFG_REG_DESIGNCAP, &designcap);
	rv = maxfg_read(sc->sc_dev, MAXFG_REG_FULLCAP, &lastfullcap);
	rv = maxfg_read(sc->sc_dev, MAXFG_REG_VCELL, &designvolt);

	lastfullcap = lastfullcap/sc->sc_rsns;

	sc->sc_bif.units = ACPI_BIF_UNITS_MA;
	sc->sc_bif.dcap = designcap*5/sc->sc_rsns;
	//sc->sc_bif.lfcap = lastfullcap*5/sc->sc_rsns;
	sc->sc_bif.lfcap = lastfullcap*5;
	sc->sc_bif.btech = 1;		/* rechargable battery */
	sc->sc_bif.dvol = 0; //((uint32_t)designvolt>>3)*625/1000;
	sc->sc_bif.wcap = (uint32_t)lastfullcap*15/100;
	sc->sc_bif.lcap = (uint32_t)lastfullcap*10/100;
	sc->sc_bif.gra1 = 70;		/* granularity 1 (warn to low) */
	sc->sc_bif.gra2 = 70;		/* granularity 2 (full to warn) */

	memcpy(sc->sc_bif.model, MAXFG_BIF_MODEL, strlen(MAXFG_BIF_MODEL));
	memcpy(sc->sc_bif.serial, MAXFG_BIF_SERIAL, strlen(MAXFG_BIF_SERIAL));
	memcpy(sc->sc_bif.type, MAXFG_BIF_TYPE, strlen(MAXFG_BIF_TYPE));
	memcpy(sc->sc_bif.oeminfo, MAXFG_BIF_OEMINFO, strlen(MAXFG_BIF_OEMINFO));

	return (0);
}

static int
maxfg_detach(device_t dev)
{
	struct maxfg_softc *sc;
	sc = device_get_softc(dev);

	return (0);
}

static int 
maxfg_read(device_t dev, uint8_t reg, uint16_t *val)
{
	struct maxfg_softc *sc;
	struct iic_msg msg[2];
	uint16_t addr = iicbus_get_addr(dev) << 1;

	sc = device_get_softc(dev);

	msg[0].slave = addr;
	msg[0].flags = IIC_M_WR | IIC_M_NOSTOP;
	msg[0].len = 1;
	msg[0].buf = &reg;

	msg[1].slave = addr;
	msg[1].flags = IIC_M_RD;
	msg[1].len = sizeof(uint16_t);
	msg[1].buf = (uint8_t *)val;

	return (iicbus_transfer(dev, msg, 2));
}

int
maxfg_get_bif(device_t dev, struct acpi_bif *bif)
{
	struct maxfg_softc *sc;
	sc = device_get_softc(dev);

	bif->units = sc->sc_bif.units;
	bif->dcap = sc->sc_bif.dcap;
	bif->lfcap = sc->sc_bif.lfcap;
	bif->btech = sc->sc_bif.btech;
	bif->dvol = sc->sc_bif.dvol;
	bif->wcap = sc->sc_bif.wcap;
	bif->lcap = sc->sc_bif.lcap;
	bif->gra1 = sc->sc_bif.gra1;
	bif->gra2 = sc->sc_bif.gra2;

	strncpy(bif->model, sc->sc_bif.model, sizeof(sc->sc_bif.model));
	strncpy(bif->serial, sc->sc_bif.serial, sizeof(sc->sc_bif.serial));
	strncpy(bif->type, sc->sc_bif.type, sizeof(sc->sc_bif.type));
	strncpy(bif->oeminfo, sc->sc_bif.oeminfo, sizeof(sc->sc_bif.oeminfo));

	return (0);
}

int
maxfg_get_bst(device_t dev, struct acpi_bst *bst)
{
	struct maxfg_softc *sc;
	uint16_t remcap , volt, rate;
	sc = device_get_softc(dev);

	/* 
	 * The value is stored in terms of μVh and must be divided by the
	 * application sense-resistor value to determine remaining capacity in
	 * mAh 
	 */
	maxfg_read(dev, MAXFG_REG_REMCAP, &remcap);
	maxfg_read(dev, MAXFG_REG_AVG_VOLT, &volt);
	maxfg_read(dev, MAXFG_REG_AVG_CUR, &rate);

	device_printf(dev, "battery %02d%%\n", maxfg_remaining(dev));	

	/* fuel guage can't detect power, always say we are discharging */
	bst->state = ACPI_BATT_STAT_DISCHARG;
	bst->cap = remcap*5/sc->sc_rsns;
	bst->volt = (((uint32_t)volt >> 3) * 625)/1000;	/* 0.625mV per lsb */
	bst->rate = (((uint32_t)rate * 15625)/10000)/sc->sc_rsns; /* 1.5625uV/rsense per lsb */

	return (0);
}

static device_method_t maxfg_methods[] = {
	DEVMETHOD(device_probe,		maxfg_probe),
	DEVMETHOD(device_attach,	maxfg_attach),
	DEVMETHOD(device_detach,	maxfg_detach),

	/* ACPI battery interface */                        
	DEVMETHOD(acpi_batt_get_status, maxfg_get_bst),
	DEVMETHOD(acpi_batt_get_info, maxfg_get_bif),
	DEVMETHOD_END
};

static driver_t maxfg_driver = {
	.name = "maxfg",
	.methods = maxfg_methods,
	.size = sizeof(struct maxfg_softc)
};

static devclass_t maxfg_devclass;
DRIVER_MODULE(maxfg, iicbus, maxfg_driver, maxfg_devclass, NULL , NULL);

MODULE_DEPEND(maxfg, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(maxfg, 1);
