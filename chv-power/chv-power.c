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

static MALLOC_DEFINE(M_CHVPWR, "chv-power", "CHV Power Driver");


struct chvpower_child {
	uint8_t address;
	char *resource_source;
};

struct chvpower_softc {
	device_t			sc_dev;
	ACPI_HANDLE			sc_handle;

	uint8_t				sc_iicchild_count;
	struct chvpower_child 		sc_iicchildren[IIC_CHILD_MAX];

	//max170xx
	//fusb
	//pi3usb3xxxxx
};

static char *chvpower_hids[] = {
	"INT33FE",     
	NULL            
};                  

static int chvpower_probe(device_t);
static int chvpower_attach(device_t);
static int chvpower_detach(device_t);

static ACPI_STATUS acpi_collect_i2c_resources(ACPI_RESOURCE *, void *);
static device_t iicbus_for_acpi_resource_source(device_t, device_t , const char *);

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
     */

	struct chvpower_softc *sc = device_get_softc(dev);
    device_t parent;
	ACPI_STATUS status;
	sc = device_get_softc(dev);
	sc->sc_dev = dev;
	device_t iicbus;

	device_printf(dev, "walking acpi tree\n");
	sc->sc_handle = acpi_get_handle(dev);
	status = AcpiWalkResources(sc->sc_handle, "_CRS", 
		acpi_collect_i2c_resources, dev);

	device_printf(dev, "walking acpi tree - DONE\n");

	if (sc->sc_iicchild_count != 4)
		return (ENXIO);

    parent = device_get_parent(dev);

	device_printf(dev, "searching for iicbus\n");
	iicbus = iicbus_for_acpi_resource_source(dev, parent,
		sc->sc_iicchildren[1].resource_source);
	device_printf(dev, "searching for iicbus - DONE\n");
	
	device_t child = BUS_ADD_CHILD(iicbus, 0, "max170xx", -1);
	if (child != NULL)
		iicbus_set_addr(child, sc->sc_iicchildren[1].address);

	return (ENXIO);
	//return (0);
}

static device_t
iicbus_for_acpi_resource_source(device_t dev, device_t bus, const char *name)
{
	int unit;
	devclass_t dc;
	dc = devclass_find("iicbus");

	device_printf(dev, "searching for child source matching: %s\n", name);

	for (unit = 0; unit < devclass_get_maxunit(dc); unit++) {
		device_t iicbus = device_find_child(bus, "iicbus", unit);
		if (iicbus == NULL)
			continue;

		ACPI_HANDLE handle = acpi_get_handle(iicbus);
		if (handle == NULL) {
			continue;
		}

		device_printf(dev, "checking: %s\n", acpi_name(handle));
		if (strcmp(acpi_name(handle), name) == 0) 
			return iicbus;
	}
	return NULL;
}

static ACPI_STATUS
acpi_collect_i2c_resources(ACPI_RESOURCE *res, void *context)
{
	struct link_count_request *req;
	device_t dev = (device_t)context;
	struct chvpower_softc *sc;
	sc = device_get_softc(dev);

	req = (struct link_count_request *)context;
	device_printf(dev, "resource of number: %x\n", res->Type);

	switch (res->Type) {
	case ACPI_RESOURCE_TYPE_SERIAL_BUS:
		device_printf(dev, "serial resource number: %x\n"
			"rev id: %x type: %x producer consumer: %x slave mode: %x "
			"connection sharing: %x type rev id: %x type data len: %x "
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
			"resource source, index: %x, str len: %x, str ptr: %p str:\n\t%s\n",
			res->Data.CommonSerialBus.ResourceSource.Index,
			res->Data.CommonSerialBus.ResourceSource.StringLength,
			res->Data.CommonSerialBus.ResourceSource.StringPtr,
			res->Data.CommonSerialBus.ResourceSource.StringPtr);

		int type = res->Data.CommonSerialBus.Type;
		switch (type) {
		case ACPI_RESOURCE_SERIAL_TYPE_I2C:
			device_printf(dev, "i2c device," 
				"access mode: %x addr: %x, connection speed %x\n", 
				res->Data.I2cSerialBus.AccessMode,
				res->Data.I2cSerialBus.SlaveAddress,
				res->Data.I2cSerialBus.ConnectionSpeed);

				if (sc->sc_iicchild_count < IIC_CHILD_MAX) {
					sc->sc_iicchildren[sc->sc_iicchild_count].address = 
						res->Data.I2cSerialBus.SlaveAddress;
/*
		sc->sc_iicchildren[sc->sc_iicchild_count].resource_source = 
			strndup(res->Data.CommonSerialBus.ResourceSource.StringPtr,
				(size_t)res->Data.CommonSerialBus.ResourceSource.StringLength, 
				M_CHVPWR);
*/
/*
	sc->sc_iicchildren[sc->sc_iicchild_count].resource_source = 
		malloc((size_t)res->Data.CommonSerialBus.ResourceSource.StringLength  * sizeof(uint8_t), 
		M_CHVPWR, M_WAITOK);
	memcpy(sc->sc_iicchildren[sc->sc_iicchild_count].resource_source, 
		res->Data.CommonSerialBus.ResourceSource.StringPtr, 
		(size_t)res->Data.CommonSerialBus.ResourceSource.StringLength);
*/

					sc->sc_iicchild_count++;
				}
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
				"edian: %x data bits %x, stop bits %x "
				"flow ctrl %x parity %x lines enabled %x "
				"rx fifo size %x tx fifo size %x "
				"default baud %x\n ",
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
