#
# Copyright (C) 2011 Samsung Electronics Co., Ltd.
#              http://www.samsung.com/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#modify by zth
#
####################################

if [ -z $1 ] #$1参数的长度为0条件就为真
then
    echo "usage: ./sd_fusing.sh <SD Reader's device file>"
    exit 0
fi

if [ -b $1 ] #如果$1存在且是一个块特殊文件则为真
then
    echo "$1 reader is identified."
else
    echo "$1 is NOT identified."
    exit 0
fi

####################################
#<verify device>
#basename是去除目录后剩下的名字(例如:basename/usr/bin/sort  输出"sort")
BDEV_NAME=`basename $1` #$1就是SD卡
BDEV_SIZE=`cat /sys/block/${BDEV_NAME}/size` #查看SD卡的大小

if [ ${BDEV_SIZE} -le 0 ]; then #BDEV_SIZE如果小于等于0则条件为真
	echo "Error: NO media found in card reader."
	exit 1
fi

if [ ${BDEV_SIZE} -gt 32000000 ]; then #BDEV_SIZE如果大于32000000则条件为真
	echo "Error: Block device size (${BDEV_SIZE}) is too large"
	exit 1
fi

####################################
# check files

E4412_UBOOT=$2 #$2就是make编程生成的led.bin
MKBL2=./mkbl2

if [ ! -f ${E4412_UBOOT} ]; then #如果E4412_UBOOT存在且是一个普通文件则为真
	echo "Error: led.bin NOT found, please build it & try again."
	exit -1
fi

if [ ! -f ${MKBL2} ]; then
	echo "Error: can not find host tool - mkbl2, stop."
	exit -1
fi

#<make bl2>
${MKBL2} ${E4412_UBOOT} bl2.bin 14336   #(14kb*1024=14336b)

####################################
# fusing images

#SD卡分区的位置
signed_bl1_position=1
bl2_position=17
uboot_position=49
tzsw_position=705
#<BL1 fusing>
echo "---------------------------------------"
echo "BL1 fusing"
dd iflag=dsync oflag=dsync if=./E4412_N.bl1.bin of=$1 seek=$signed_bl1_position

#<BL2 fusing>
echo "---------------------------------------"
echo "BL2 fusing"
dd iflag=dsync oflag=dsync if=./bl2.bin of=$1 seek=$bl2_position

#<u-boot fusing>
#echo "---------------------------------------"
#echo "u-boot fusing"
#dd iflag=dsync oflag=dsync if=${E4412_UBOOT} of=$1 seek=$uboot_position

#<flush to disk>
sync

####################################
#<Message Display>
echo "---------------------------------------"
echo "U-boot image is fused (at `date +%T`) successfully."
echo "Eject SD card and insert it again."

