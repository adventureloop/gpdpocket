#!/bin/sh 

sudo rm -rf /boot/kernel.udpopt
cp -r kernel/boot/kernel /boot/kernel.udpopt
sudo nextboot -k kernel.udpopt

sudo shutdown -r +2s &

exit 0
