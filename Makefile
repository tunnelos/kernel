.ONESHELL:

CFLAGS = -Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -I./bootboot/dist/ -I./include -I/usr/include
LDFLAGS = -nostdlib -nostartfiles -T link.ld
STRIPFLAGS = -s -K freemem -K mmio -K fb -K bootboot -K environment -K initstack
OSNAME = tunnel

all: $(OSNAME).x86_64.elf iso fullclean

$(OSNAME).x86_64.elf: main.c screen.c stdio.c tunnel.c
	gcc $(CFLAGS) -mno-red-zone -c main.c -o main.o
	gcc $(CFLAGS) -mno-red-zone -c screen.c -o screen.o
	gcc $(CFLAGS) -mno-red-zone -c cstring.c -o cstring.o
	gcc $(CFLAGS) -mno-red-zone -c cint.c -o cint.o
	gcc $(CFLAGS) -mno-red-zone -c stdio.c -o stdio.o
	gcc $(CFLAGS) -mno-red-zone -c tunnel.c -o tunnel.o
	ld -r -b binary -o fonts/text.o fonts/text.psf
	ld $(LDFLAGS) main.o stdio.o screen.o cstring.o cint.o fonts/text.o -o $(OSNAME).x86_64.elf
	strip $(STRIPFLAGS) $(OSNAME).x86_64.elf
	readelf -hls $(OSNAME).x86_64.elf > $(OSNAME).x86_64.txt

iso:
	rm -rv iso
	mkdir iso
	mkdir iso/tmp
	mkdir iso/tmp/mkbootimg
	cp bootboot/mkbootimg iso/tmp/mkbootimg/ -r
	mkdir iso/tmp/sys
	cp tunnelconfig/* .
	cp config iso/tmp/sys/
	cd iso/tmp/mkbootimg/mkbootimg
	make all
	cp ../../../../$(OSNAME).x86_64.elf . -r
	cp ../../../../$(OSNAME).json . -r
	./mkbootimg check $(OSNAME).x86_64.elf
	mkdir boot
	cp ../../sys boot/ -r
	cp $(OSNAME).x86_64.elf boot/sys/core -r
	./mkbootimg $(OSNAME).json $(OSNAME).x86_64.img
	echo "Copying... Please, wait."
	cp $(OSNAME).x86_64.img ../../../$(OSNAME).x86_64.img -rv
	cd ../../../
	rm -rf tmp
	iat $(OSNAME).x86_64.img $(OSNAME).x86_64.iso
	du -h $(OSNAME).x86_64.iso
	du -h ../$(OSNAME).x86_64.elf
	rm -rfv $(OSNAME).x86_64.img
	cd ../
	rm -rf config tunnel.json
clean:
	rm -rf *.o
fullclean: clean
	rm -rf *.elf *.iso
	cp iso/$(OSNAME).x86_64.iso .
	rm -rf iso
turron:
	qemu-system-x86_64 --boot d --cdrom $(OSNAME).x86_64.iso