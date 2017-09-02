#!/bin/sh                                               
                                                        
set -e                                                  
                                                        
echo "building kernel modules"                          
echo "  - building pi3usb driver"                     

cd pi3usb
make                                                    
cd -
                                                        
echo "  - building fusb3 driver"                     

cd fusb3
make                                                    
cd -

echo "  - building max170xx driver"                     

cd max170xx
make                                                    
cd -

                                                        
echo "- building chv pwr driver"                       

cd chv-power
make
cd -

echo "loading kernel modules"                           
sudo kldload ./max170xx/max170xx.ko
sudo kldload ./fusb3/fusb3.ko
sudo kldload ./pi3usb/pi3usb.ko
sudo kldload ./chv-power/chv-power.ko

sudo kldunload chv-power.ko
sudo kldunload pi3usb.ko
sudo kldunload fusb3.ko
sudo kldunload max170xx.ko
