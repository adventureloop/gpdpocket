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

#include "../max170xx/max170xx_var.h"

static int max170xx_acpi_probe(device_t);
static int max170xx_acpi_attach(device_t);
static int max170xx_acpi_detach(device_t);

static int
max170xx_acpi_probe(device_t dev)
{
	device_set_desc(dev, "Maxim max170xx Fuel Guage");
	return (0);
}

static int
max170xx_acpi_attach(device_t dev)
{
	struct max170xx_softc *sc = device_get_softc(dev);
	int error;

	sc->sc_dev = dev;

	error = max170xx_attach(dev);
	if (error)
		max170xx_detach(dev);

	return (error);
}

static int
max170xx_acpi_detach(device_t dev)
{
	return (0);
}

static device_method_t max170xx_methods[] = {
	DEVMETHOD(device_probe,		max170xx_acpi_probe),
	DEVMETHOD(device_attach,	max170xx_acpi_attach),
	DEVMETHOD(device_detach,	max170xx_acpi_detach),

	/* ACPI battery interface */                        
//	DEVMETHOD(acpi_batt_get_status, max170xx_get_bst),
//	DEVMETHOD(acpi_batt_get_info, max170xx_get_bif),  
	DEVMETHOD_END
};

static driver_t max170xx_acpi_driver = {
	.name = "max170xx_acpi",
	.methods = max170xx_methods,
	.size = sizeof(struct max170xx_softc)
};

static devclass_t max170xx_acpi_devclass;
DRIVER_MODULE(max170xx_acpi, iicbus, max170xx_acpi_driver, max170xx_acpi_devclass, NULL , NULL);

MODULE_DEPEND(max170xx_acpi, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(max170xx_acpi, 1);
