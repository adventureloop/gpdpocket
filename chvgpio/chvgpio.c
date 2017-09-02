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
#include <sys/types.h>
#include <sys/malloc.h>

#include <machine/bus.h>
#include <machine/resource.h>

#include "opt_acpi.h"

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>

#include <dev/iicbus/iicbus.h>
#include <dev/iicbus/iiconf.h>

#define IIC_CHILD_MAX 4
#define DEBUG 0

static MALLOC_DEFINE(M_CHVGPIO, "chvgpio", "CHV GPIO Driver");


struct chvgpio_child {
	uint8_t address;
	char *resource_source;
};

struct chvgpio_softc {
	device_t				sc_dev;
	ACPI_HANDLE				sc_handle;
};

static char *chvgpio_hids[] = {
	"INT33FF",     
	NULL            
};                  

static int chvgpio_probe(device_t);
static int chvgpio_attach(device_t);
static int chvgpio_detach(device_t);

static ACPI_STATUS acpi_collect_gpio(ACPI_RESOURCE *, void *);

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
	device_printf(dev, "chvgpio attach\n");

	struct chvgpio_softc *sc;
	ACPI_STATUS status;
	int uid;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;

	status = acpi_GetInteger(sc->sc_handle, "_UID", &uid);
	if (ACPI_FAILURE(status)) {
		device_printf(dev, "failed to read _UID\n");
//		return (ENXIO);
	} else 
		device_printf(dev, "_UID %d\n", uid);

	sc->sc_handle = acpi_get_handle(dev);
	status = AcpiWalkResources(sc->sc_handle, "_CRS", 
		acpi_collect_gpio, dev);

	return (0);
}

static ACPI_STATUS
acpi_collect_gpio(ACPI_RESOURCE *res, void *context)
{
	struct link_count_request *req;
	device_t dev = (device_t)context;
	struct chvgpio_softc *sc;
	sc = device_get_softc(dev);

	req = (struct link_count_request *)context;
	device_printf(dev, "resource of number: %x\n", res->Type);

	switch (res->Type) {
	case ACPI_RESOURCE_TYPE_PIN_CONFIG:
		device_printf(dev, "serial resource number: %x\n"
			"rev id: %x producer consumer: %x sharable: %x "
			"pin config type: %x pin config value: %x pin table len: %x "
			"vendor len: %x\n",
			res->Type,
			res->Data.PinConfig.RevisionId,
			res->Data.PinConfig.ProducerConsumer,
			res->Data.PinConfig.Sharable,
			res->Data.PinConfig.PinConfigType,
			res->Data.PinConfig.PinConfigValue,
			res->Data.PinConfig.PinTableLength,
			res->Data.PinConfig.VendorLength);

		device_printf(dev, 
			"resource source, index: %x, str len: %x, str:\n\t%s\n",
			res->Data.PinConfig.ResourceSource.Index,
			res->Data.PinConfig.ResourceSource.StringLength,
			res->Data.PinConfig.ResourceSource.StringPtr);

//UINT16                          *PinTable;       
//UINT8                           *VendorData;     
		break;
	default:
		break;
	}
	return (AE_OK);
}

static int
chvgpio_detach(device_t dev)
{
	struct chvgpio_softc *sc;
	sc = device_get_softc(dev);

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
	DEVMETHOD(device_probe,		chvgpio_probe),
	DEVMETHOD(device_attach,	chvgpio_attach),
	DEVMETHOD(device_detach,	chvgpio_detach),
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
MODULE_VERSION(chvgpio, 1);
