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

struct chvpower_softc {
	device_t			sc_dev;
};


static int chvpower_probe(device_t);
static int chvpower_attach(device_t);
static int chvpower_detach(device_t);

static int
chvpower_probe(device_t dev)
{
	device_set_desc(dev, "Intel Cherry View power device");
	return (0);
}

static int
chvpower_attach(device_t dev)
{
//	struct chvpower_softc *sc = device_get_softc(dev);

	return (0);
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
		uprintf("chvpower touch screen KLD loaded.\n");
		break;
	case MOD_UNLOAD:
		uprintf("chvpower touch screen KLD unloaded.\n");
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
DRIVER_MODULE(chvpower, iicbus, chvpower_driver, chvpower_devclass, chvpower_driver_loaded, NULL);

MODULE_DEPEND(chvpower, iicbus, IICBUS_MINVER, IICBUS_PREFVER, IICBUS_MAXVER);
MODULE_VERSION(chvpower, 1);
