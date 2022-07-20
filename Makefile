.ONESHELL:

LDFLAGS_X86_64 = -nostdlib -nostartfiles -T link.ld
STRIPFLAGS_X86_64 = -K mmio -K fb -K bootboot -K environment -K initstack
OSNAME = tunnel
FILELIST_X86_64 = main.o screen.o stdio.o tunnel.o shell.o cstring.o cint.o panic.o mm.o     \
				  smt.o keyboard_ps2.o tools.o serial.o idt.o idt_ASM.o pit.o window.o fs.o  \
	    		  window_welcome.o shell_mouse.o ide.o event.o fpu_ASM.o path.o hal.o ui.o   \
	    		  cpuid_tools_ASM.o sse_ASM.o avx_ASM.o sse.o uhci.o cmos.o test.o arch.o    \
	    		  easter.o math.o desktop.o pit_ASM.o tools_ASM.o pic_ASM.o rtc.o stdlib.o   \
	    		  encoder.o sort.o cJSON.o cJSON_Utils.o systemconf.o pic.o trnd.o unitype.o \
	    		  placeholder.o system_JSON.o tunnel_JSON.o network.o api.o nmi.o
FONTLIST =        text_PSF.o gui_PSF.o

all: clean $(OSNAME).x86_64.iso

setup:
	@bash setup.sh
arch:
	@echo "Avaliable architectures"
	@echo "x86_64"
help:
	@echo "Avaliable commands"
	@echo "make arch - show avaliable architectures"
	@echo "make all - compile code into bootable files"
	@echo "make setup - set up compiling environment"
	@echo "make iso - generate iso file"
	@echo "make clean - clean from object files"
	@echo "make fullclean - execute clean and remove target files"
	@echo "make vhd - generate vhd file"
$(OSNAME).x86_64.iso:
	@rm -rf temp/
	@mkdir temp
	@mkdir temp/debug
	@mkdir temp/iso
	@mkdir temp/iso/tmp
	@mkdir temp/iso/tmp/mkbootimg
	@mkdir temp/iso/tmp/sys
	@cp -r arch/x86_64/* temp/
	@cp -r temp/linker/* temp/
	@cd temp

	@bash finder.sh
	@ld $(LDFLAGS_X86_64) $(FILELIST_X86_64) $(FONTLIST) -o $(OSNAME).x86_64.elf || false
	@strip $(STRIPFLAGS_X86_64) $(OSNAME).x86_64.elf
	@readelf -hls $(OSNAME).x86_64.elf > $(OSNAME).x86_64.txt
	@rm -rf fonts_compiled
	@mkdir fonts_compiled
	@mv fonts/*.o fonts_compiled/
	@cp $(OSNAME).x86_64.elf debug/$(OSNAME).x86_64.elf
	@cd debug
	@objdump -D tunnel.x86_64.elf > tunnel.x86_64.txt
	@cd ..
	@cp bootboot/mkbootimg iso/tmp/mkbootimg/ -r
	@cp tunnelconfig/* .
	@cp config iso/tmp/sys/
	@cd iso/tmp/mkbootimg/mkbootimg
	@make all
	@cp ../../../../$(OSNAME).x86_64.elf . -r
	@cp ../../../../$(OSNAME).json . -r
	@./mkbootimg check $(OSNAME).x86_64.elf
	@mkdir boot
	@cp ../../sys boot/ -r
	@cp $(OSNAME).x86_64.elf boot/sys/core -r
	@./mkbootimg $(OSNAME).json $(OSNAME).x86_64.img
	@mv $(OSNAME).x86_64.img ../../../$(OSNAME).x86_64.img -v
	@cd ../../../
	@rm -rf tmp
	@iat $(OSNAME).x86_64.img $(OSNAME).x86_64.iso
	@du -h $(OSNAME).x86_64.iso
	@du -h ../$(OSNAME).x86_64.elf
	@rm -rfv $(OSNAME).x86_64.img
	@cd ../
	@rm -rf config tunnel.json

	@cp -r debug ../targets_debug
	@cp -r iso ../targets_executeable
	@cd ..
	@rm -rf temp
clean:
	@rm -rf *.o fonts/*.o tunnelconfig/*.o
	@rm -rf temp
fullclean:
	@rm -rf *.elf *.iso
	@cp iso/$(OSNAME).x86_64.iso .
	@rm -rf iso
vhd:
	@dd if=/dev/zero of=VHD.img bs=1M count=512