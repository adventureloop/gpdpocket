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

static MALLOC_DEFINE(M_CHVPWR, "chv-power", "CHV Power Driver");


struct chvpower_child {
	uint8_t address;
	char *resource_source;
};

struct chvpower_softc {
	device_t				sc_dev;
	ACPI_HANDLE				sc_handle;

	uint8_t					sc_iicchild_count;
	struct chvpower_child 	sc_iicchildren[IIC_CHILD_MAX];

	device_t				sc_max170xx;
	device_t				sc_fusb3;
	device_t				sc_pi3usb;
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

	struct chvpower_softc *sc;
    device_t parent;
	ACPI_STATUS status;
	device_t iicbus;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;

	sc->sc_handle = acpi_get_handle(dev);
	status = AcpiWalkResources(sc->sc_handle, "_CRS", 
		acpi_collect_i2c_resources, dev);

	if (sc->sc_iicchild_count != 4)
		return (ENXIO);

    parent = device_get_parent(dev);

#define MAX170XX 0
#if MAX170XX
	/* 
	 * The String in the child acpi is missing an underscore (\_SB. vs \/_SB_)
	 * compensate for this manually, free the alloced string and replace it 
	 * with the correct one.
	 */
	free(sc->sc_iicchildren[1].resource_source, M_CHVPWR);
	sc->sc_iicchildren[1].resource_source = "\\_SB_.PCI0.I2C1";

	iicbus = iicbus_for_acpi_resource_source(dev, parent,
		sc->sc_iicchildren[1].resource_source);

	if (iicbus != NULL) {
		device_t child = BUS_ADD_CHILD(iicbus, 0, "max170xx", -1);
		if (child != NULL) {
			iicbus_set_addr(child, sc->sc_iicchildren[1].address);
			sc->sc_max170xx = child;
			bus_generic_attach(iicbus);
			device_printf(dev, "added max170xx child\n");
		} else
			device_printf(dev, "failed to add child max170xx\n");
	}
#endif
#define FUSB3 0
#if FUSB3
	/*
	 * FUSB seems to require an irq, deal with that hassle.
	 */

	/* 
	 * The String in the child acpi is missing an underscore (\_SB. vs \/_SB_)
	 * compensate for this manually, free the alloced string and replace it 
	 * with the correct one.
	 */
	free(sc->sc_iicchildren[2].resource_source, M_CHVPWR);
	sc->sc_iicchildren[2].resource_source = "\\_SB_.PCI0.I2C1";

	iicbus = iicbus_for_acpi_resource_source(dev, parent,
		sc->sc_iicchildren[2].resource_source);

	if (iicbus != NULL) {
		device_t child = BUS_ADD_CHILD(iicbus, 0, "fusb3", -1);
		if (child != NULL) {
			iicbus_set_addr(child, sc->sc_iicchildren[2].address);
			sc->sc_fusb3 = child;
			bus_generic_attach(iicbus);
			device_printf(dev, "added fusb3 child\n");
		} else
			device_printf(dev, "failed to add child fusb3\n");
	}
#endif
#define PI3USB 1
#if PI3USB
	/* 
	 * The String in the child acpi is missing an underscore (\_SB. vs \/_SB_)
	 * compensate for this manually, free the alloced string and replace it 
	 * with the correct one.
	 */
	free(sc->sc_iicchildren[3].resource_source, M_CHVPWR);
	sc->sc_iicchildren[3].resource_source = "\\_SB_.PCI0.I2C1";

	iicbus = iicbus_for_acpi_resource_source(dev, parent,
		sc->sc_iicchildren[3].resource_source);

	if (iicbus != NULL) {
		device_t child = BUS_ADD_CHILD(iicbus, 0, "pi3usb", -1);
		if (child != NULL) {
			iicbus_set_addr(child, sc->sc_iicchildren[3].address);
			sc->sc_pi3usb = child;
			bus_generic_attach(iicbus);
			device_printf(dev, "added pi3usb child\n");
		} else
			device_printf(dev, "failed to add child pi3usb\n");
	}
#endif

	return (0);
}

static device_t
iicbus_for_acpi_resource_source(device_t dev, device_t bus, const char *name)
{
	int unit;
	devclass_t dc;
	dc = devclass_find("iicbus");
	if (dc == NULL) {
		device_printf(dev, "devclas_find returned NULL\n");
		return NULL;
	}

//	device_printf(dev, "searching for child source matching: %s\n", name);

	for (unit = 0; unit < devclass_get_maxunit(dc); unit++) {
		device_t ig4iic = device_find_child(bus, "ig4iic_acpi", unit);
		if (ig4iic == NULL) {
//			device_printf(dev, "no ig4 for unit: %d\n", unit);
			continue;
		}

		device_t iicbus = device_find_child(ig4iic, "iicbus", -1);
		if (iicbus == NULL) {
//			device_printf(dev, "no iicbus on ig4iic-acpi%d\n", unit);
			continue;
		}

		ACPI_HANDLE handle = acpi_get_handle(ig4iic);
		if (handle == NULL) {
//			device_printf(dev, "no acpi handle for ig4iic_acpi%d\n", unit);
			continue;
		}

//		device_printf(dev, "checking: %s\n", acpi_name(handle));
		if (strcmp(acpi_name(handle), name) == 0) 
			return iicbus;
	}
	return NULL;
}

static ACPI_STATUS
acpi_collect_i2c_resources(ACPI_RESOURCE *res, void *context)
{
	int type;
	struct link_count_request *req;
	device_t dev = (device_t)context;
	struct chvpower_softc *sc;
	sc = device_get_softc(dev);

	req = (struct link_count_request *)context;
#if DEBUG
	device_printf(dev, "resource of number: %x\n", res->Type);
#endif
	switch (res->Type) {
	case ACPI_RESOURCE_TYPE_SERIAL_BUS:
#if DEBUG
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
			"resource source, index: %x, str len: %x, str:\n\t%s\n",
			res->Data.CommonSerialBus.ResourceSource.Index,
			res->Data.CommonSerialBus.ResourceSource.StringLength,
			res->Data.CommonSerialBus.ResourceSource.StringPtr);
#endif
		type = res->Data.CommonSerialBus.Type;
		switch (type) {
		case ACPI_RESOURCE_SERIAL_TYPE_I2C:
#if DEBUG
			device_printf(dev, "i2c device," 
				"access mode: %x addr: %x, connection speed %x\n", 
				res->Data.I2cSerialBus.AccessMode,
				res->Data.I2cSerialBus.SlaveAddress,
				res->Data.I2cSerialBus.ConnectionSpeed);
#endif
				if (sc->sc_iicchild_count < IIC_CHILD_MAX) {
					sc->sc_iicchildren[sc->sc_iicchild_count].address = 
						res->Data.I2cSerialBus.SlaveAddress;

					sc->sc_iicchildren[sc->sc_iicchild_count].resource_source = 
						strndup(res->Data.CommonSerialBus.ResourceSource.StringPtr,
						(size_t)res->Data.CommonSerialBus.ResourceSource.StringLength, 
						M_CHVPWR);

					sc->sc_iicchild_count++;
				}
			break;
		case ACPI_RESOURCE_SERIAL_TYPE_SPI:
#if DEBUG
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
#endif
			break;
		case ACPI_RESOURCE_SERIAL_TYPE_UART:
#if DEBUG
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
#endif
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
	int child; 
	struct chvpower_softc *sc;
	sc = device_get_softc(dev);

	if (sc->sc_max170xx)
		device_delete_child(device_get_parent(sc->sc_max170xx), sc->sc_max170xx);
#if FUSB3
	if (sc->sc_fusb3)
		device_delete_child(device_get_parent(sc->sc_fusb3), sc->sc_fusb3);
#endif
#if PI3USB
	if (sc->sc_pi3usb)
		device_delete_child(device_get_parent(sc->sc_pi3usb), sc->sc_pi3usb);
#endif

	for (child = 0; child < IIC_CHILD_MAX; child++) {
		if (child == 1)		//HACK TODO REMOVE
			continue;
		if (sc->sc_iicchildren[child].resource_source) {
			free(sc->sc_iicchildren[child].resource_source, M_CHVPWR);
			sc->sc_iicchildren[child].resource_source = NULL;
		}
	}

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
