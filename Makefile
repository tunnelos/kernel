.ONESHELL:

LDFLAGS_X86_64  = -nostdlib -nostartfiles -T link.ld
LDFLAGS_AARCH64 = -nostdlib -nostartfiles -T link.ld
STRIPFLAGS_X86_64 = -K mmio -K fb -K bootboot -K environment -K initstack
OSNAME = tunnel
FILELIST_X86_64 =  main.o stdio.o tunnel.o shell.o cstring.o cint.o panic.o mm.o nmi.o api.o  \
				   smt.o keyboard_ps2.o tools.o serial.o idt.o idt_ASM.o pit.o window.o fs.o  \
	    		   window_welcome.o shell_mouse.o ide.o event.o fpu_ASM.o path.o hal.o ui.o   \
	    		   cpuid_tools_ASM.o sse_ASM.o avx_ASM.o sse.o uhci.o cmos.o test.o arch.o    \
	    		   easter.o math.o desktop.o pit_ASM.o tools_ASM.o pic_ASM.o rtc.o stdlib.o   \
	    		   encoder.o sort.o cJSON.o cJSON_Utils.o systemconf.o pic.o trnd.o unitype.o \
	    		   placeholder.o system_JSON.o tunnel_JSON.o network.o cppfuncs.o cpptest.o
FILELIST_AARCH64 = boot_ASM.o armio.o cint.o math.o stdlib.o system_JSON.o main.o
FONTLIST =         text_PSF.o gui_PSF.o

aarch64_target: clean $(OSNAME).aarch64.elf
x86_64_target:  clean $(OSNAME).x86_64.iso
all: x86_64_target aarch64_target
	@mkdir build
	@mkdir build/debug
	@mkdir build/executeable
	@cp targets_debug/* build/debug -r
	@cp targets_executeable/* build/executeable -r
	@rm targets_executeable targets_debug -r
	@zip targets.zip build -r9
	@rm debug iso fonts_compiled -rf

setup:
	@bash setup.sh
arch:
	@echo "Avaliable architectures"
	@echo "x86_64 aarch64"
help:
	@echo "make arch - show avaliable architectures"
	@echo "make all - compile code into bootable files"
	@echo "make setup - set up compiling environment"
	@echo "make iso - generate iso file"
	@echo "make clean - clean	
	@echo "Avaliable commandn from object files"
	@echo "make fullclean - execute clean and remove target files"
	@echo "make vhd - generate vhd file"
$(OSNAME).aarch64.elf:
	@rm -rf temp
	@mkdir -p temp
	@mkdir -p temp/debug
	@mkdir -p targets_debug
	@mkdir -p targets_executeable
	@cp -r arch/aarch64/* temp/
	@cp -r temp/linker/* temp/

	@cd temp
	@bash finder.sh
	@aarch64-linux-gnu-ld $(LDFLAGS_AARCH64) $(FILELIST_AARCH64) $(FONTLIST) -o $(OSNAME).aarch64.elf || false
	@aarch64-linux-gnu-objdump -D $(OSNAME).aarch64.elf > $(OSNAME).aarch64.txt
	
	@cp $(OSNAME).aarch64.elf ../targets_executeable/
	@cp $(OSNAME).aarch64.txt ../targets_debug

	@cd ..
	@rm -rf temp
$(OSNAME).x86_64.iso:
	@rm -rf temp
	@mkdir -p targets_debug
	@mkdir -p targets_executeable
	@mkdir -p temp
	@mkdir -p temp/debug
	@mkdir -p temp/iso
	@mkdir -p temp/iso/tmp
	@mkdir -p temp/iso/tmp/mkbootimg
	@mkdir -p temp/iso/tmp/sys
	@cp -r arch/x86_64/* temp/
	@cp -r temp/linker/* temp/
	@cd temp

	@bash finder.sh
	@ld $(LDFLAGS_X86_64) $(FILELIST_X86_64) $(FONTLIST) -o $(OSNAME).x86_64.elf || false
	@strip $(STRIPFLAGS_X86_64) $(OSNAME).x86_64.elf
	@readelf -hls $(OSNAME).x86_64.elf > $(OSNAME).x86_64.txt
	@cp $(OSNAME).x86_64.elf debug/$(OSNAME).x86_64.elf
	@cd debug
	@objdump -D $(OSNAME).x86_64.elf > $(OSNAME).x86_64.txt
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

	@cp -r debug/* ../targets_debug
	@cp -r iso/* ../targets_executeable
	@cd ..
	@rm -rf temp
clean:
	@rm -rf *.o fonts/*.o tunnelconfig/*.o temp targets_debug targets_executeable build targets.zip
fullclean:
	@rm -rf *.elf *.iso
	@cp iso/$(OSNAME).x86_64.iso .
	@rm -rf iso
vhd:
	@dd if=/dev/zero of=VHD.img bs=1M count=512