#!/bin/sh                                               
                                                        
set -e                                                  
                                                        
echo "building kernel modules"                          
echo "  - building max170xx driver"                     

cd ../max170xx
make                                                    
cd -
                                                        
echo "- building chv pwr driver"                       

make

echo "loading kernel modules"                           
sudo kldload ../max170xx/max170xx.ko
sudo kldload ./chv-power.ko


sudo kldunload max170xx.ko
sudo kldunload chv-power.ko
