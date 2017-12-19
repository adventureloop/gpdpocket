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

#define	MAX170xx_STATUS	0x00
#define	MAX170xx_TEMP	0x08
#define MAX170xx_SOCAV	0x0E	//state of charge
#define	MAX170xx_TTE	0x11	//time to empty
#define	MAX170xx_CONFIG 0x1D
#define	MAX170xx_SOCVF	0xFF	//State Of Charge

static int max170xx_read(device_t, uint8_t, uint16_t *);
static int max170xx_write(device_t, uint8_t, uint16_t );
static void max170xx_dumpreg(device_t);

static int
max170xx_remaining(device_t dev)
{
	uint16_t socvf = 0;
	
	max170xx_read(dev, MAX170xx_SOCVF, &socvf);
	return ( ((socvf >> 8) * 100) + (((socvf & 0x00FF) *100)/256) )/100;
}

void 
max170xx_dumpreg(device_t dev) 
{
	uint16_t status = 0;	//POR 0x0002
	uint16_t temp = 0;		//POR 0x1600
	uint16_t tte = 0;		//POR 0x0000
	uint16_t config = 0;	//POR 0x2530
	uint16_t socvf = 0;		//POR 0x0000
	uint16_t socav = 0;		//POR 0x3200

	uint8_t remain = 0;		

	max170xx_read(dev, MAX170xx_STATUS, &status);
	max170xx_read(dev, MAX170xx_TEMP, &temp);
	max170xx_read(dev, MAX170xx_SOCAV, &socav);
	max170xx_read(dev, MAX170xx_TTE, &tte);
	max170xx_read(dev, MAX170xx_CONFIG, &config);
	max170xx_read(dev, MAX170xx_SOCVF, &socvf);

	device_printf(dev, "fuel guage read: STATUS: %x TEMP: %x SOCKAV: %x "
		"TTE: %x CONFIG: %x SOCVF: %x\n", 
		status, temp, socav, tte, config, socvf);

	device_printf(dev, "\tstatus %b\n", status,
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

	device_printf(dev, "\tconfig %b\n", config,
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

	remain = max170xx_remaining(dev);	
	device_printf(dev, "battery %d%%\n", remain);
}

static int
max170xx_probe(device_t dev)
{
	device_set_desc(dev, "Maxim max170xx Fuel Guage");
	return (0);
}

int
max170xx_attach(device_t dev)
{
	struct max170xx_softc *sc = device_get_softc(dev);
	int rv;

	sc->sc_dev = dev;
	sc->sc_addr = MAX170xx_SADDR << 1;

	uint16_t status = 0;	//POR 0x0002
	rv = max170xx_read(sc->sc_dev, MAX170xx_STATUS, &status);
	if ( rv != 0) {
		device_printf(sc->sc_dev, "first read failed code: %d %d\n", rv, iic2errno(rv));
		return ENXIO;
	}

	max170xx_dumpreg(sc->sc_dev);

	devclass_t maxdc;
	maxdc = device_get_devclass(dev);

	if (maxdc == NULL)
		device_printf(dev, " dev classs is null\n");
	else
		device_printf(dev, " dev classs is %s\n", devclass_get_name(maxdc));

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

static int
max170xx_get_bif(device_t dev, struct acpi_bif *bif)
{
    struct max170xx_softc *sc;

    sc = device_get_softc(dev);

    bif->units = ACPI_BIF_UNITS_MA;	//ACPI_BIF_UNITS_MW
    bif->dcap = 7000;		//design cap
    bif->lfcap = 7000;	//last full cap
    bif->btech = 1;	//battery technology
    bif->dvol = 10000;		//design voltage
    bif->wcap = 0;		//warn cap
    bif->lcap = 0;		// low cap
    bif->gra1 = 70;		//granularity 1 (warn to low)
    bif->gra2 = 70;		//granularity 1 (full to warn)
	// this way handy https://docs.microsoft.com/en-us/windows-hardware/design/device-experiences/acpi-battery-and-power-subsystem-firmware-implementation
/*
    strncpy(bifp->model, sc->bif.model, sizeof(sc->bif.model));		// max17047
    strncpy(bifp->serial, sc->bif.serial, sizeof(sc->bif.serial));	// version
    strncpy(bifp->type, sc->bif.type, sizeof(sc->bif.type));		// can be null
    strncpy(bifp->oeminfo, sc->bif.oeminfo, sizeof(sc->bif.oeminfo));	// can be null
*/
    return (0);
}

static int
max170xx_get_bst(device_t dev, struct acpi_bst *bst)
{
    struct acpi_cmbat_softc *sc;

    sc = device_get_softc(dev);

/* 
 * ACPI_BATT_STAT_DISCHARG     0x0001
 * ACPI_BATT_STAT_CHARGING     0x0002
 * ACPI_BATT_STAT_CRITICAL     0x0004
 * ACPI_BATT_STAT_NOT_PRESENT;
 */
    bst->state = ACPI_BATT_STAT_DISCHARG;
	bst->rate = 1;
	bst->cap = max170xx_remaining(dev);
	bst->volt = 1;

    return (0);
}

static device_method_t max170xx_methods[] = {
	DEVMETHOD(device_probe,		max170xx_probe),

	/* ACPI battery interface */                        
	DEVMETHOD(acpi_batt_get_status, max170xx_get_bst),
	DEVMETHOD(acpi_batt_get_info, max170xx_get_bif),  
	DEVMETHOD_END
};

static driver_t max170xx_driver = {
	.name = "battery",
	.methods = max170xx_methods,
	.size = sizeof(struct max170xx_softc)
};

static devclass_t max170xx_devclass;
DRIVER_MODULE(max170xx, iicbus, max170xx_driver, max170xx_devclass, NULL , NULL);

MODULE_DEPEND(max170xx, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(max170xx, 1);
