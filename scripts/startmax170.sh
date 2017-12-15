#!/bin/sh

git pull

max170=`kldstat | grep max170xx`
chvpower=`kldstat | grep chvpower`

if [ ! -z "$max170" ]
then
	kldunload max170xx.ko
fi

if [ ! -z "$chvpower" ]
then
	kldload chvpower.ko
fi

make -C max170xx
make -C chvpower

kldload max170xx/max170xx.ko
kldload chvpower/chvpower.ko
