#!/bin/sh

BASEDIR="/usr/home/tom/code/gpdpocket"
SRCDIR=$BASEDIR/src
OBJDIR=$BASEDIR/obj
KERNDIR=$BASEDIR/kernel

pushd $SRCDIR

env MAKEOBJDIRPREFIX=$OBJDIR \
    make installkernel -j4 \
	-DKERNFAST KODIR=$KERNDIR

popd
