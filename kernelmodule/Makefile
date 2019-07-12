kmodname = chardev
obj-m += $(kmodname).o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	rm *.o *.symvers *.order *.mod.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

insmod:
	sudo insmod ./$(kmodname).ko

rmmod:
	sudo rmmod $(kmodname)

modinfo:
	modinfo $(kmodname).ko

showmessage:
	cat /var/log/syslog

mknod:
	sudo mknod /dev/chardev c 243 0

rmnod:
	sudo rm /dev/chardev


