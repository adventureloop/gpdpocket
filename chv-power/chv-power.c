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

#include "opt_acpi.h"

#include <contrib/dev/acpica/include/acpi.h>
#include <contrib/dev/acpica/include/accommon.h>

#include <dev/acpica/acpivar.h>

#include <dev/iicbus/iicbus.h>
#include <dev/iicbus/iiconf.h>

struct chvpower_softc {
	device_t			sc_dev;
	ACPI_HANDLE			sc_handle;
};

static char *chvpower_hids[] = {
	"INT33FE",     
	NULL            
};                  


static int chvpower_probe(device_t);
static int chvpower_attach(device_t);
static int chvpower_detach(device_t);

static ACPI_STATUS acpi_count_i2c_resources(ACPI_RESOURCE *, void *);

static int
chvpower_probe(device_t dev)
{
	if (acpi_disabled("chvpower") ||
    ACPI_ID_PROBE(device_get_parent(dev), dev, chvpower_hids) == NULL)
		return (ENXIO);

	device_set_desc(dev, "Intel Cherry View Power Nexus");
	return (0);
}

static int
chvpower_attach(device_t dev)
{
	device_printf(dev, "chvpower attach\n");

    /*
     * TODO:
     * look here:
     * https://github.com/freebsd/freebsd/blob/2df1b01611ffde3f1ce1630866cf76f4de49c7a6/sys/dev/chromebook_platform/chromebook_platform.c#L57
     *
     * - get the parent acpi bus
     * - check the handle for a unit number
     * - search the acpi parent bus for an iic driver (unit-1)
     *      - if that fails try again
     * - search the new bus for the acutal device
     */

	struct chvpower_softc *sc = device_get_softc(dev);
    device_t parent;
	ACPI_STATUS status;
	//int uid;

	/* getting the _UID */
	sc = device_get_softc(dev);
	sc->sc_dev = dev;

	sc->sc_handle = acpi_get_handle(dev);
	/*
	status = acpi_GetInteger(sc->sc_handle, "_UID", &uid);
	if (ACPI_FAILURE(status)) {
		device_printf(dev, "failed to read _UID\n");
		return (ENXIO);
	}
	*/
	/*
	 * walk acpi resource tree, something like this maybe:
	 * https://github.com/freebsd/freebsd/blob/386ddae58459341ec567604707805814a2128a57/sys/dev/acpica/acpi_pci_link.c#L521
	 */

    //unit = acpi_get_unitsomething(handle)	I wonder if it is resource I wish for
	status = AcpiWalkResources(sc->sc_handle, "_CRS", 
		acpi_count_i2c_resources, dev);

    parent = device_get_parent(dev);
	return (ENXIO);

    //if !acpi_parent
     //   return ENOFRIENDS

    //iicbus = device_find_child(parent, "iicbus", unit)
	//return (0);
}

static ACPI_STATUS
acpi_count_i2c_resources(ACPI_RESOURCE *res, void *context)
{
	struct link_count_request *req;
	device_t dev = (device_t)context;

	req = (struct link_count_request *)context;
	device_printf(dev, "resource of number: %x\n", res->Type);

	switch (res->Type) {
	case ACPI_RESOURCE_TYPE_SERIAL_BUS:
		device_printf(dev, "serial resource number: %x\n"
			"rev id: %x type: %x producer consumer: %x slave mode: %x"
			"connection sharing: %x type rev id: %x type data len: %x"
			"vendor len: %x\n",
			res->Type,
			res->Data.CommonSerialBus.RevisionId,
			res->Data.CommonSerialBus.Type,
			res->Data.CommonSerialBus.ProducerConsumer,
			res->Data.CommonSerialBus.SlaveMode,
			res->Data.CommonSerialBus.ConnectionSharing,
			res->Data.CommonSerialBus.TypeRevisionId,
			res->Data.CommonSerialBus.TypeDataLength,
			res->Data.CommonSerialBus.VendorLength);

		device_printf(dev, 
			"resource source, index: %x, str len %x, str:\n\t%s\n",
			res->Data.CommonSerialBus.ResourceSource.Index,
			res->Data.CommonSerialBus.ResourceSource.StringLength,
			res->Data.CommonSerialBus.ResourceSource.StringPtr);

		//ACPI_RESOURCE_SOURCE            ResourceSource; \
		//UINT8                           *VendorData;


		int type = res->Data.CommonSerialBus.Type;
		switch (type) {
		case ACPI_RESOURCE_SERIAL_TYPE_I2C:
			device_printf(dev, "i2c device," 
				"access mode: %x addr: %x, connection speed %x\n", 
				res->Data.I2cSerialBus.AccessMode,
				res->Data.I2cSerialBus.SlaveAddress,
				res->Data.I2cSerialBus.ConnectionSpeed);
			break;
		case ACPI_RESOURCE_SERIAL_TYPE_SPI:
			device_printf(dev, "SPI device"
				"wire mode: %x polarity: %x bit length %x clock phase %x "
				"clock polarity %x device selection %x connection speed %x\n",
				res->Data.SpiSerialBus.WireMode,
				res->Data.SpiSerialBus.DevicePolarity,
				res->Data.SpiSerialBus.DataBitLength,
				res->Data.SpiSerialBus.ClockPhase,
				res->Data.SpiSerialBus.ClockPolarity,
				res->Data.SpiSerialBus.DeviceSelection,
				res->Data.SpiSerialBus.ConnectionSpeed);
			break;
		case ACPI_RESOURCE_SERIAL_TYPE_UART:
			device_printf(dev, "UART device"
				"edian: %x data bits %x, stop bits %x"
				"flow ctrl %x parity %x lines enabled %x"
				"rx fifo size %x tx fifo size %x"
				"default baud %x\n",
				res->Data.UartSerialBus.Endian,
				res->Data.UartSerialBus.DataBits,
				res->Data.UartSerialBus.StopBits,
				res->Data.UartSerialBus.FlowControl,
				res->Data.UartSerialBus.Parity,
				res->Data.UartSerialBus.LinesEnabled,
				res->Data.UartSerialBus.RxFifoSize,
				res->Data.UartSerialBus.TxFifoSize,
				res->Data.UartSerialBus.DefaultBaudRate);
			
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return (AE_OK);
}

static int
chvpower_detach(device_t dev)
{
	return (0);
}

static int
chvpower_driver_loaded(struct module *m, int what, void *arg)
{
	int err = 0;

	switch (what) {
	case MOD_LOAD:
		uprintf("chvpower KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("chvpower KLD unloaded.\n");
		break;
	default:
		err = EOPNOTSUPP;
		break;
	}
	return(err);
}

static device_method_t chvpower_methods[] = {
	DEVMETHOD(device_probe,		chvpower_probe),
	DEVMETHOD(device_attach,	chvpower_attach),
	DEVMETHOD(device_detach,	chvpower_detach),
	DEVMETHOD_END
};

static driver_t chvpower_driver = {
	.name = "chvpower",
	.methods = chvpower_methods,
	.size = sizeof(struct chvpower_softc)
};

static devclass_t chvpower_devclass;
DRIVER_MODULE(chvpower, acpi, chvpower_driver, chvpower_devclass, chvpower_driver_loaded, NULL);

MODULE_DEPEND(chvpower, acpi, 1, 1, 1);
//MODULE_DEPEND(chvpower, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(chvpower, 1);
