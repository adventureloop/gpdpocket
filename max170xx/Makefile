SRCS=bus_if.h iicbus_if.h device_if.h opt_acpi.h acpi_if.h ${max170xx_acpi}  max170xx.c
KMOD=max170xx

.if ${MACHINE_CPUARCH} == "amd64" || ${MACHINE_CPUARCH} == "i386"
max170xx_acpi=	max170xx_acpi.c
.endif

.include <bsd.kmod.mk>
