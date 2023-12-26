CUR_DIR = $(shell pwd)
KERN_DIR =$(CUR_DIR)/../../out_vexpress_arm32_target/

obj-m	+= kmap.o
	
all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  HOSTCC=/usr/bin/gcc -C $(KERN_DIR) M=`pwd` modules

.PHONY: clean	
clean:
	rm  *.o *.ko *.mod *.mod.c *.order *.symvers

