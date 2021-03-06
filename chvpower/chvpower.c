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

/*
 *     Macros for driver mutex locking
 */
#define CHVPOWER_LOCK(_sc)               mtx_lock(&(_sc)->sc_mtx)
#define CHVPOWER_UNLOCK(_sc)             mtx_unlock(&(_sc)->sc_mtx)
#define CHVPOWER_LOCK_INIT(_sc) \
        mtx_init(&_sc->sc_mtx, device_get_nameunit((_sc)->sc_dev), \
        "chvpower", MTX_DEF)
#define CHVPOWER_LOCK_DESTROY(_sc)       mtx_destroy(&(_sc)->sc_mtx)
#define CHVPOWER_ASSERT_LOCKED(_sc)      mtx_assert(&(_sc)->sc_mtx, MA_OWNED)
#define CHVPOWER_ASSERT_UNLOCKED(_sc)    mtx_assert(&(_sc)->sc_mtx, MA_NOTOWNED)

#define IIC_CHILD_MAX 4

static MALLOC_DEFINE(M_CHVPWR, "chvpower", "Cherry View Power Driver");

struct chvpower_child {
	uint8_t address;
	char *resource_source;
};

struct chvpower_softc {
	device_t		sc_dev;
	ACPI_HANDLE		sc_handle;
	struct mtx		sc_mtx;

	uint8_t			sc_iicchild_count;
	struct chvpower_child 	sc_iicchildren[IIC_CHILD_MAX];

	device_t		sc_maxfg;
	device_t		sc_bqreg;
};

static char *chvpower_hids[] = {
	"INT33FE",     
	NULL            
};                  

static int chvpower_probe(device_t);
static int chvpower_attach(device_t);
static int chvpower_detach(device_t);

static ACPI_STATUS acpi_collect_i2c_resources(ACPI_RESOURCE *, void *);
static device_t iicbus_for_acpi_resource_source(device_t, device_t, const char *, const char *);

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
	struct chvpower_softc *sc;
	device_t parent;
	ACPI_STATUS status;
	device_t iicbus;

	sc = device_get_softc(dev);
	sc->sc_dev = dev;
	sc->sc_handle = acpi_get_handle(dev);

	CHVPOWER_LOCK_INIT(sc);

	status = AcpiWalkResources(sc->sc_handle, "_CRS", acpi_collect_i2c_resources, dev);

	if (sc->sc_iicchild_count != 4)
		return (ENXIO);

	parent = device_get_parent(dev);
#if 0
	/* 
	 * The String in the child acpi is missing an underscore (\_SB. vs \_SB_)
	 * compensate for this manually, free the alloced string and replace it 
	 * with the correct one.
	 */
	free(sc->sc_iicchildren[1].resource_source, M_CHVPWR);
	sc->sc_iicchildren[1].resource_source = "\\_SB_.PCI0.I2C1";

	iicbus = iicbus_for_acpi_resource_source(dev, parent, "ig4iic_acpi",
		sc->sc_iicchildren[1].resource_source);

	if (iicbus != NULL) {
		device_t child = BUS_ADD_CHILD(iicbus, 0, "maxfg", -1);
		if (child != NULL) {
			iicbus_set_addr(child, sc->sc_iicchildren[1].address);
			sc->sc_maxfg = child;
			bus_generic_attach(iicbus);

			if (acpi_battery_register(dev) != 0) {
				device_printf(dev, "failed to register battery\n");
				return (ENXIO);                                 
			}                                                   
		} else
			device_printf(dev, "failed to add maxfg child\n");
	} 
#endif
#if 1
	/* 
	 * The String in the child acpi is missing an underscore (\_SB. vs \_SB_)
	 * compensate for this manually, free the alloced string and replace it 
	 * with the correct one.
	 */
	device_printf(dev, "bqreg resource_source %s\n", sc->sc_iicchildren[0].resource_source);
	free(sc->sc_iicchildren[0].resource_source, M_CHVPWR);
	sc->sc_iicchildren[0].resource_source = "\\_SB_.PCI0.I2C1";

	iicbus = iicbus_for_acpi_resource_source(dev, parent, "ig4iic_acpi",
		sc->sc_iicchildren[0].resource_source);

	if (iicbus != NULL) {
		device_t child = BUS_ADD_CHILD(iicbus, 0, "bqreg", -1);
		if (child != NULL) {
			device_printf(dev, "bqreg addr %x\n", sc->sc_iicchildren[0].address);
			iicbus_set_addr(child, sc->sc_iicchildren[0].address);
			sc->sc_bqreg = child;
			bus_generic_attach(iicbus);
		} else
			device_printf(dev, "failed to add bqreg child\n");
	} 
#endif
	return (0);
}

static device_t
iicbus_for_acpi_resource_source(device_t dev, device_t acpidev,
	const char *iicdevname, const char *name)
{
	int unit;
	devclass_t dc;
	dc = devclass_find("iicbus");
	if (dc == NULL)
		return NULL;

	for (unit = 0; unit < devclass_get_maxunit(dc); unit++) {
		device_t iicdev = device_find_child(acpidev, iicdevname, unit);
		if (iicdev == NULL) {
			continue;
		}

		device_t iicbus = device_find_child(iicdev, "iicbus", -1);
		if (iicbus == NULL) {
			continue;
		}

		ACPI_HANDLE handle = acpi_get_handle(iicdev);
		if (handle == NULL) {
			continue;
		}

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
	switch (res->Type) {
	case ACPI_RESOURCE_TYPE_SERIAL_BUS:
		type = res->Data.CommonSerialBus.Type;
		switch (type) {
		case ACPI_RESOURCE_SERIAL_TYPE_I2C:
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

	CHVPOWER_LOCK(sc);

	if (sc->sc_maxfg)
		device_delete_child(device_get_parent(sc->sc_maxfg), sc->sc_maxfg);

	CHVPOWER_LOCK_DESTROY(sc);

	for (child = 0; child < IIC_CHILD_MAX; child++) {
		if (child == 0)		//HACK TODO REMOVE
			continue;
		if (sc->sc_iicchildren[child].resource_source) {
			free(sc->sc_iicchildren[child].resource_source, M_CHVPWR);
			sc->sc_iicchildren[child].resource_source = NULL;
		}
	}

	return (0);
}

static int 
chvpower_get_bst(device_t dev, struct acpi_bst *bst)
{
	struct chvpower_softc *sc;
	int error;

	sc = device_get_softc(dev);
	error = 0;

//	CHVPOWER_LOCK(sc);

	if (sc->sc_maxfg)
		error = ACPI_BATT_GET_STATUS(sc->sc_maxfg, bst);
//	CHVPOWER_UNLOCK(sc);
	return (error);
}

static int 
chvpower_get_bif(device_t dev, struct acpi_bif *bif)
{
	struct chvpower_softc *sc;
	int error;

	sc = device_get_softc(dev);
	error = 0;

//	CHVPOWER_LOCK(sc);

	if (sc->sc_maxfg)
		error = ACPI_BATT_GET_INFO(sc->sc_maxfg, bif);

//	CHVPOWER_UNLOCK(sc);
	return (error);
}

static device_method_t chvpower_methods[] = {
	DEVMETHOD(device_probe,		chvpower_probe),
	DEVMETHOD(device_attach,	chvpower_attach),
	DEVMETHOD(device_detach,	chvpower_detach),

	/* ACPI battery interface */
	DEVMETHOD(acpi_batt_get_status, chvpower_get_bst),
	DEVMETHOD(acpi_batt_get_info, chvpower_get_bif),

	DEVMETHOD_END
};

static driver_t chvpower_driver = {
	.name = "battery",
	.methods = chvpower_methods,
	.size = sizeof(struct chvpower_softc)
};

static devclass_t chvpower_devclass;
DRIVER_MODULE(chvpower, acpi, chvpower_driver, chvpower_devclass, NULL, NULL);

MODULE_DEPEND(chvpower, acpi, 1, 1, 1);
MODULE_DEPEND(chvpower, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(chvpower, 1);
