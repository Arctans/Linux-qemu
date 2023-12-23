#!/bin/bash
##########################################################################
# File Name: run_vexpress_arm32.sh
# Author: Arctan
# Created Time: 2023年01月19日 星期四 17时15分21秒
#########################################################################

CUR_PATH=`pwd`

if [ ! -d ./kmodules_arm32 ]; then
	mkdir kmodules_arm32
fi

NO_GRAPHIC="-nographic "

CPU_NR="-smp 2 "

run_vexpress()
{
	qemu-system-arm \
		-M vexpress-a9 \
		$CPU_NR \
		-m 1024M \
		-kernel ./out_vexpress_arm32_target/arch/arm/boot/zImage \
		-dtb ./out_vexpress_arm32_target/arch/arm/boot/dts/vexpress-v2p-ca9.dtb \
		 $NO_GRAPHIC \
		-append "root=/dev/mmcblk0 rw console=ttyAMA0" \
		--fsdev local,id=kmod_dev,path=./kmodules_arm32,security_model=none\
		-device virtio-9p-device,fsdev=kmod_dev,mount_tag=kmod_mount\
		-net user,hostfwd=tcp::2222-:22 \
		-net nic \
		-sd $1 \
		$DEBUG

}
run_imx6dl()
{
	qemu-system-arm \
		-M sabrelite \
		$CPU_NR \
		-m 1024M \
		-kernel ./out_imx6dl_arm32_target/arch/arm/boot/zImage \
		-dtb ./out_imx6dl_arm32_target/arch/arm/boot/dts/imx6dl-sabresd.dtb \
		 $NO_GRAPHIC \
		-append "console=ttymxc0 rootfstype=ext4 rootwait root=/dev/mmcblk3"  \
		--fsdev local,id=kmod_dev,path=./kmodules_arm32,security_model=none\
		-device sd-card,drive=mysdcard \
		-net user,hostfwd=tcp::2222-:22 \
		-net nic \
		-drive if=none,file=$1,format=raw,id=mysdcard \
		$DEBUG

}



main_start()
{
	if [ "debug" = "$3" ]; then
		echo "Enable qemu debug"
		DEBUG="-s -S"
	fi

	case $2 in
		ubuntu22_04)
			rootfs="$CUR_PATH/rootfs/ubuntu22.04_arm32/rootfs_ubuntu22_04.ext4"
			;;
		debian)
			rootfs="$CUR_PATH/rootfs/debian_arm32/rootfs_debian_arm32.ext4"
			;;
		busybox)
			rootfs="$CUR_PATH/rootfs/busybox/busybox_rootfs.ext4"
			;;
	esac

	case $1 in

		imx6dl)
			run_imx6dl $rootfs
			;;
		vexpress)
			run_vexpress $rootfs
			;;
	esac
}

if [  -n $1 ];then
	echo "sh run_arm32.sh 板卡 文件系统 [debug]"
	echo "sh run_arm32.sh (imx6dl | vexpress) (ubuntu22_04 | debian | busybox)"
fi
BOARD_INFO=$1
ROOTFS_INFO=$2
DEBUG_INFO=$3
main_start $BOARD_INFO $ROOTFS_INFO $DEBUG_INFO
