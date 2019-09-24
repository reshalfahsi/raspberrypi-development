# Raspberry Pi Kernel Module #

This kernel module will print the student's name and number in the terminal

## How to Use ##

### Phase I : ###

Please, run this following command in terminal: 

~~~~
$ make clean
$ make all
$ make insmod
$ dmesg
~~~~

### Phase II : ###

After run the `dmesg` check the ** major number ** which kernel module that was inserted then change the command in [Makefile](https://github.com/reshalfahsi/raspberrypi-development/blob/master/kernelmodule/Makefile) to command that the terminal has said before. Then run this following command:

~~~~
$ make mknod
$ cd app
$ make clean
$ make all
$ make run
~~~~
