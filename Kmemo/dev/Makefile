obj-m += sample.o
CFLAGS_sample.o := -fno-stack-protector -fno-omit-frame-pointer
temp = "5.15.90.4-microsoft-standard-WSL2"
#temp  = "6.5.0-rc5-microsoft-standard-WSL2+"
all:
	make -C /lib/modules/${temp}/build M=$(PWD) modules

clean:
	make -C /lib/modules/${temp}/build M=$(PWD) clean

