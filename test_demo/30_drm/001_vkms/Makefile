CUR_DIR = $(shell pwd)
KERN_DIR =$(CUR_DIR)/../../../out/out_vexpress_arm32_target/

obj-m	+= vkms_drv.o
TARGET  += vkms_drv.ko
	
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  HOSTCC=/usr/bin/gcc -C $(KERN_DIR) M=`pwd` modules
	cp $(TARGET) $(CUR_DIR)/../../../kmodules_arm32/

.PHONY: clean	
clean:
	rm  *.o *.ko *.mod *.mod.c *.order *.symvers

