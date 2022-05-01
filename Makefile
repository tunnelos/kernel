.ONESHELL:

LDFLAGS = -nostdlib -nostartfiles -T link.ld
STRIPFLAGS = -K mmio -K fb -K bootboot -K environment -K initstack
OSNAME = tunnel
FILELIST =  main.o screen.o stdio.o tunnel.o shell.o cstring.o cint.o panic.o mm.o \
			smt.o keyboard_ps2.o tools.o serial.o idt.o idta.o pit.o window.o fs.o \
			window_welcome.o shell_mouse.o ide.o idea.o event.o fpua.o path.o ui.o \
			color.o tunnelconfig/system.o cpuid_toolsa.o ssea.o avxa.o sse.o uhci.o\
			easter.o math.o desktop.o pita.o toolsa.o pic.o pica.o rtc.o nmi.o
FONTLIST = fonts/text.o fonts/gui.o

all: $(OSNAME).x86_64.elf iso fullclean

$(OSNAME).x86_64.elf:
	rm -rfv debug 
	mkdir debug
	bash compile.sh
	ld $(LDFLAGS) $(FILELIST) $(FONTLIST) -o $(OSNAME).x86_64.elf
	strip $(STRIPFLAGS) $(OSNAME).x86_64.elf
	readelf -hls $(OSNAME).x86_64.elf > $(OSNAME).x86_64.txt
	rm -rfv fonts_compiled
	mkdir fonts_compiled
	mv fonts/*.o fonts_compiled/
	cp $(OSNAME).x86_64.elf debug/$(OSNAME).x86_64.elf
	cd debug
	objdump -D tunnel.x86_64.elf > tunnel.x86_64.txt
	cd ..
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
	mv $(OSNAME).x86_64.img ../../../$(OSNAME).x86_64.img -v
	cd ../../../
	rm -rf tmp
	iat $(OSNAME).x86_64.img $(OSNAME).x86_64.iso
	du -h $(OSNAME).x86_64.iso
	du -h ../$(OSNAME).x86_64.elf
	rm -rfv $(OSNAME).x86_64.img
	cd ../
	rm -rf config tunnel.json
clean:
	rm -rf *.o fonts/*.o tunnelconfig/*.o
fullclean: clean
	rm -rf *.elf *.iso
	cp iso/$(OSNAME).x86_64.iso .
	rm -rf iso
vhd:
	dd if=/dev/zero of=VHD.img bs=1M count=512
turron:
	qemu-system-x86_64 --cpu kvm64-v1,+avx -d int --boot d --cdrom $(OSNAME).x86_64.iso -m 256M -smp 3 -serial stdio -drive file=VHD.img,index=0,if=ide,format=raw -s -S