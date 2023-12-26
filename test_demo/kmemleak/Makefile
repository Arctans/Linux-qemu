CUR_DIR = $(shell pwd)
KERN_ARM32_DIR =$(CUR_DIR)/../../out_vexpress_arm32_target/
KERN_ARM64_DIR =$(CUR_DIR)/../../out_vexpress_arm64_target/

obj-m	+= kmemleak_test.o
TARGET  += kmemleak_test.ko
	
arm:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-  HOSTCC=/usr/bin/gcc -C $(KERN_ARM32_DIR) M=`pwd` modules
	cp $(TARGET) $(CUR_DIR)/../../kmodules_arm32/

arm64:
	make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu-  HOSTCC=/usr/bin/gcc -C $(KERN_ARM64_DIR) M=`pwd` modules
	cp $(TARGET) $(CUR_DIR)/../../kmodules_arm64/

.PHONY: clean	
clean:
	rm  *.o *.ko *.mod.c *.order *.symvers *.mod

