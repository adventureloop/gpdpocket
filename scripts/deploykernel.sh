#!/usr/local/bin/zsh

set -e

SRC="/home/tom/code/freebsd/src/"
DSTHOST="pm"

pushd $SRC

echo "Building Kernel"
env MAKEOBJDIRPREFIX=/usr/home/tom/code/freebsd/obj \
	make -j4 -DKERNFAST buildkernel

echo "Installing locally Kernel"
sudo /home/tom/code/freebsd/scripts/installkernel.sh

echo "Client:"
echo "Client:\tcleaning up old kernel"

ssh $DSTHOST rm -rf kernel

echo "Client:\tcopying new kernel"
scp -r ../kernel DSTHOST:
