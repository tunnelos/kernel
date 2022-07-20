CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -nostartfiles -Wno-shift-count-overflow -I./include"

for f in *.c ; do 
    echo Compiling ${f%.c}.c ...;
    aarch64-linux-gnu-gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o; 
done
for g in *.psf ; do 
    echo Linking ${g%.psf}.psf ...;
    aarch64-linux-gnu-ld -r -b binary -o ${g%.psf}_PSF.o ${g%.psf}.psf; 
done
for j in *.json; do
    echo Linking ${j%.json}.json;
    aarch64-linux-gnu-ld -r -b binary -o ${j%.json}_JSON.o ${j%.json}.json; 
done
for h in *.asm ; do
    echo Assembling ${h%.asm}.asm ...;
    aarch64-linux-gnu-as ${h%.asm}.asm -mtune cortex-a57.cortex-a53 -o ${h%.asm}_ASM.o
    nasm -felf64 ${h%.asm}.asm -o ${h%.asm}_ASM.o; 
done
