#!/bin/sh

INTERVAL=10		# sleep time in seconds 
MAX=65
MED=55
MIN=45

GPIODEV="/dev/gpioc1"
LOWPINNAME="GPIO_DFX0_PAD"
HIGHPINNAME="GPIO_DFX3_PAD"

requirements()
{
	printf "gpdfand.sh requires:\n"	
	printf "\tcoretemp kernel module\n"
	printf "\tchvgpio kernel module\n"
	exit 1
}

# On the GPD Pocket we need coretemp(for core temperatures) and chvgpio to be
# loaded. Check if they are, then verify that the device and the pin names we
# need are available.
start()
{
	coretemp=`kldstat | grep coretemp`
	chvgpio=`kldstat | grep chvgpio`

	if [ -z "$coretemp" ]
	then
		printf "coretemp kernel module not loaded\ntry: \n\t#kldload coretemp\n"
		requirements
	fi

	if [ -z "$chvgpio" ]
	then
		printf "chvgpio kernel module not loaded\ntry: \n\t#kldload chvgpio\n"
		requirements
	fi

	if [ ! -e $GPIODEV ]
	then
		printf "$GPIODEV file not found, there is a problem somewhere\n"
		requirements
	fi

	gpioctl -f $GPIODEV -N $LOWPINNAME

	if [ $? -ne 0 ]
	then
		printf "$GPIODEV Pin called $LOWPINGNAME not found, there is a problem somewhere\n"
		requirements
	fi

	gpioctl -f $GPIODEV -N $HIGHPINNAME
	if [ $? -ne 0 ]
	then
		printf "$GPIODEV Pin called $HIGHPINGNAME not found, there is a problem somewhere\n"
		requirements
	fi


	echo "starting gpdfand..."
}


# read any temperature values from sysctl if coretemp is loaded there will be
# 4, one for each core. On other platforms ministat might complain about too
# few entries
readtemp()
{
	sysctl -a | grep temperature | sed -e 's/C//' | ministat -n -C 2 | \
		tail -n 1 | awk '{print $4}'
}

setfan()
{
	gpioctl -f /dev/gpioc1 -N GPIO_DFX0_PAD $1
	gpioctl -f /dev/gpioc1 -N GPIO_DFX3_PAD $2
}

siginfohandler()
{
	echo `date +"%Y%m%d%H%M%S"` $temp $speed
}

trap "siginfohandler" SIGINFO

start

temp=0
speed="0 0"

while true
do
	temp=$(readtemp)
	if [ $temp -gt $MAX ] #or temp == 0
	then
		speed="1 1"
	elif [ $temp -ge $MED ]
	then
		speed="0 1"
	elif [ $temp -ge $MIN ]
	then
		speed="1 0"
	else 
		speed="0 0"
	fi

	setfan $speed
	sleep $INTERVAL
done
