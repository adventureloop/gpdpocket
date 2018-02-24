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

#include <contrib/dev/acpica/include/acpi.h>

#include <dev/acpica/acpivar.h>
#include <dev/acpica/acpiio.h>

#include "max170xx_var.h"

#define MAX170xx_SADDR	0x36

#define	MAX170xx_REG_STATUS	0x00
#define MAX170xx_REG_SALRT_TH	0x03	//
#define	MAX170xx_REG_TEMP	0x08	// MSB +1C
#define MAX170xx_REG_VCELL	0x09	// 0.625mV per div bottom 3 bits don't cate
#define MAX170xx_REG_FULLCAP	0x10	// calculated full cap in uVh
#define MAX170xx_REG_SOCAV	0x0E	//state of charge
#define	MAX170xx_REG_TTE	0x11	//time to empty
#define MAX170xx_REG_DESIGNCAP	0x18	// design capacity in uVh
//#define MAX170xx_REG_AVGVCELL	0x19
#define	MAX170xx_REG_CONFIG 0x1D
#define	MAX170xx_REG_REMCAP	0x1F	//remaining capacity in uVh
#define MAX170xx_REG_VERSION	0x21	//
#define MAX170xx_REG_VFOCV	0xFB	//raw open-circuit volt- age output of the voltage fuel gauge
#define	MAX170xx_REG_SOCVF	0xFF	//State Of Charge

static int max170xx_read(device_t, uint8_t, uint16_t *);
static int max170xx_write(device_t, uint8_t, uint16_t );
static void max170xx_dumpreg(device_t);

static int
max170xx_remaining(device_t dev)
{
	uint16_t socvf = 0;
	
	max170xx_read(dev, MAX170xx_REG_SOCVF, &socvf);
	return ( ((socvf >> 8) * 100) + (((socvf & 0x00FF) *100)/256) )/100;
}

void 
max170xx_dumpreg(device_t dev) 
{
	uint16_t reg = 0;

	max170xx_read(dev, MAX170xx_REG_STATUS, &reg);
	device_printf(dev, "\tstatus %b\n", reg,
		"\10"
		"\001BER"	// Enable alert on battery removal
		"\002BEI" 	//Enable alert on battery insertion
		"\003AEN"	//Alert on fuel guage outputs
		"\004FTHRM"	//force thermistor bias switch
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

	max170xx_read(dev, MAX170xx_REG_CONFIG, &reg);
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

	max170xx_read(dev, MAX170xx_REG_SALRT_TH, &reg);
	device_printf(dev, "MAX170xx_REG_SALRT_TH %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_TEMP, &reg);
	device_printf(dev, "MAX170xx_REG_TEMP %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_VCELL, &reg);
	device_printf(dev, "MAX170xx_REG_VCELL %02x %02x\n", reg, (reg>>3) * 1000/625);

	max170xx_read(dev, MAX170xx_REG_FULLCAP, &reg);
	device_printf(dev, "MAX170xx_REG_FULLCAP %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_SOCAV, &reg);
	device_printf(dev, "MAX170xx_REG_SOCAV %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_TTE, &reg);
	device_printf(dev, "MAX170xx_REG_TTE %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_DESIGNCAP, &reg);
	device_printf(dev, "MAX170xx_REG_DESIGNCAP %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_REMCAP, &reg);
	device_printf(dev, "MAX170xx_REG_REMCAP %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_VERSION, &reg);
	device_printf(dev, "MAX170xx_REG_VERSION, %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_VFOCV, &reg);
	device_printf(dev, "MAX170xx_REG_VFOCV %02x\n", reg);

	max170xx_read(dev, MAX170xx_REG_SOCVF, &reg);
	device_printf(dev, "MAX170xx_REG_SOCVF %02x\n", reg);

	device_printf(dev, "battery %02x%%\n", max170xx_remaining(dev));	
}

static int
max170xx_probe(device_t dev)
{
	device_printf(dev, "probe\n");
	device_set_desc(dev, "Maxim max170xx Fuel Guage");
	return (0);
}

int
max170xx_attach(device_t dev)
{
	device_printf(dev, "attach\n");
	struct max170xx_softc *sc = device_get_softc(dev);
	int rv;
	uint16_t designcap, lastfullcap, designvolt;
	designcap = lastfullcap = designvolt = 0;


	sc->sc_dev = dev;
	sc->sc_addr = MAX170xx_SADDR << 1;
	sc->sc_rsns = 10;	// datasheet reccomends 0.01 ohms default sense resistor value (10 milliohms)

	uint16_t status = 0;	//POR 0x0002
	rv = max170xx_read(sc->sc_dev, MAX170xx_REG_STATUS, &status);
	if ( rv != 0) {
		device_printf(sc->sc_dev, "first read failed code: %d %d\n", rv, iic2errno(rv));
		return ENXIO;
	}

	max170xx_dumpreg(sc->sc_dev);

	rv = max170xx_read(sc->sc_dev, MAX170xx_REG_DESIGNCAP, &designcap);
	rv = max170xx_read(sc->sc_dev, MAX170xx_REG_FULLCAP, &lastfullcap);
	rv = max170xx_read(sc->sc_dev, MAX170xx_REG_VCELL, &designvolt);

	sc->sc_bif.units = ACPI_BIF_UNITS_MW;	//ACPI_BIF_UNITS_MW
	sc->sc_bif.dcap = designcap;
	sc->sc_bif.lfcap = lastfullcap;
	sc->sc_bif.btech = 1;		// rechargable battery
	sc->sc_bif.dvol = ((uint32_t)designvolt>>3)*1000/625;
	sc->sc_bif.wcap = (uint32_t)lastfullcap*100/95;
	sc->sc_bif.lcap = (uint32_t)lastfullcap*100/80;
	sc->sc_bif.gra1 = 70;		//granularity 1 (warn to low)
	sc->sc_bif.gra2 = 70;		//granularity 1 (full to warn)

	//sc->sc_bif.model = "max17042 Fuel Guage";
	//sc->sc_bif.serial = "default";
	//sc->sc_bif.type = "fuel guage";
	//sc->sc_bif.oeminfo = "null";

	return (0);
}

int
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

int
max170xx_get_bif(device_t dev, struct acpi_bif *bif)
{
    struct max170xx_softc *sc;

    sc = device_get_softc(dev);

	/*                  
	 * Just copy the data.  The only value that should change is the
	 * last-full capacity, so we only update when we get a notify that says
	 * the info has changed. 
	 */                 

	bif->units = sc->sc_bif.units;
	bif->dcap = sc->sc_bif.dcap;
	bif->lfcap = sc->sc_bif.lfcap;
	bif->btech = sc->sc_bif.btech;
	bif->dvol = sc->sc_bif.dvol;
	bif->wcap = sc->sc_bif.wcap;
	bif->lcap = sc->sc_bif.lcap;
	bif->gra1 = sc->sc_bif.gra1;
	bif->gra2 = sc->sc_bif.gra2;
/*
	strncpy(bif->model, sc->sc_bif.model, sizeof(sc->sc_bif.model));
	strncpy(bif->serial, sc->sc_bif.serial, sizeof(sc->sc_bif.serial));
	strncpy(bif->type, sc->sc_bif.type, sizeof(sc->sc_bif.type));
	strncpy(bif->oeminfo, sc->sc_bif.oeminfo, sizeof(sc->sc_bif.oeminfo));
*/
    return (0);
}

int
max170xx_get_bst(device_t dev, struct acpi_bst *bst)
{
    struct max170xx_softc *sc;
	uint16_t remcap;//, volt, rate;
    sc = device_get_softc(dev);

/* 
 * ACPI_BATT_STAT_DISCHARG     0x0001
 * ACPI_BATT_STAT_CHARGING     0x0002
 * ACPI_BATT_STAT_CRITICAL     0x0004
 * ACPI_BATT_STAT_NOT_PRESENT;
 */

	/* 
	 * The value is stored in terms of μVh and must be divided by the
	 * application sense-resistor value to determine remaining capacity in
	 * mAh 
	 */
	max170xx_read(dev, MAX170xx_REG_REMCAP, &remcap);


	//max170xx_read(dev, MAX170xx_REG_REMCAP, &volt);
	//max170xx_read(dev, MAX170xx_REG_REMCAP, &rate);

    bst->state = ACPI_BATT_STAT_DISCHARG;
	bst->cap = remcap;
	//bst->rate = rate;
	//bst->cap = remcap / sc->sc_rsns;
	//bst->volt = volt;

    return (0);
}

static device_method_t max170xx_methods[] = {
	DEVMETHOD(device_probe,		max170xx_probe),
	DEVMETHOD(device_attach,		max170xx_attach),

	/* ACPI battery interface */                        
	DEVMETHOD(acpi_batt_get_status, max170xx_get_bst),
	DEVMETHOD(acpi_batt_get_info, max170xx_get_bif),  
	DEVMETHOD_END
};

static driver_t max170xx_driver = {
	.name = "max170xx",
	.methods = max170xx_methods,
	.size = sizeof(struct max170xx_softc)
};

static devclass_t max170xx_devclass;
DRIVER_MODULE(max170xx, iicbus, max170xx_driver, max170xx_devclass, NULL , NULL);

MODULE_DEPEND(max170xx, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(max170xx, 1);
