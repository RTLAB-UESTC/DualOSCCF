ifneq ($(KERNELRELEASE),)
	acoral_link-objs  := acoral_link_main.o buddy.o
	obj-m := acoral_link.o

else
	#KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	KERNELDIR ?= /home/lovelife/software/PB11_ARM/kernel/linux-2.6.28.4  /home/lovelife/software/PB11_ARM/kernel/linux-2.6.28.4/arch/arm/mach-realview
	PWD := $(shell pwd)
	CC = arm-none-linux-gnueabi-gcc
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm acoral_link.ko acoral_link.o acoral_link.mod* built-in.o *odule*
endif
