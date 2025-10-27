obj-m += kmod_example.o

PWD := $(CURDIR)

all:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /usr/lib/modules/$(shell uname -r)/build M=$(PWD) clean
