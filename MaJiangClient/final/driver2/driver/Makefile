obj-m	:=magicKeys.o

KERNELDIR := /UP-Magic210/SRC/kernel/linux-2.6.35.7/
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers
.PHONY: all clean
