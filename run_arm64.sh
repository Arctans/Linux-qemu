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
	qemu-system-aarch64 -m 1024 -cpu cortex-a57 -M xlnx-versal-virt\
		$NO_GRAPHIC \
		$CPU_NR \
		$SMP -kernel ./out/out_vexpress_arm64_target/arch/arm64/boot/Image \
		-append "noinintrd root=/dev/vda rootfstype=ext4 rw loglevel=8 console=ttyAMA0" \
		-drive if=none,file=$1,id=hd0 \
		-device virtio-blk-device,drive=hd0 \
		--fsdev local,id=kmod_dev,path=./kmodules_arm64,security_model=none \
		-net user,hostfwd=tcp::2224-:22 \
		-net nic \
		-device virtio-9p-device,fsdev=kmod_dev,mount_tag=kmod_mount \
		$DEBUG
}
run_imx6dl()
{
	qemu-system-aarch64 -m 1024 -cpu cortex-a9 -M sabrelite \
		$NO_GRAPHIC \
		$CPU_NR \
		$SMP -kernel ./out/out_vexpress_arm64_target/arch/arm64/boot/Image \
		-append "noinintrd root=/dev/vda rootfstype=ext4 rw loglevel=8 console=ttyAMA0" \
		--fsdev local,id=kmod_dev,path=./kmodules_arm64,security_model=none \
		-net user,hostfwd=tcp::2224-:22 \
		-net nic \
		-device sd-card,drive=mysdcard \
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
			rootfs="$CUR_PATH/rootfs/ubuntu22.04_arm64/rootfs_ubuntu22_04.ext4"
			;;
		debian)
			rootfs="$CUR_PATH/rootfs/debian_arm64/rootfs_debian_arm64.ext4"
			;;
		# busybox)
		# 	rootfs="$CUR_PATH/rootfs/busybox/busybox_rootfs.ext4"
		# 	;;
	esac

	case $1 in

		imx6dl)
			# run_imx6dl $rootfs
			;;
		vexpress)
			run_vexpress $rootfs
			;;
	esac
}

if [  -n $1 ];then
	echo "sh run_arm64.sh 板卡 文件系统 [debug]"
	echo "sh run_arm64.sh (imx6dl | vexpress) (ubuntu22_04 | debian)"
fi
BOARD_INFO=$1
ROOTFS_INFO=$2
DEBUG_INFO=$3
main_start $BOARD_INFO $ROOTFS_INFO $DEBUG_INFO
