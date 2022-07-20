CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -nostartfiles -Wno-shift-count-overflow -I./include"

for f in *.c ; do
    if [ "$f" != '*.c' ]
    then
        echo Compiling ${f%.c}.c in aarch64 codebase ...;
        aarch64-linux-gnu-gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o; 
    fi
done
for g in *.psf ; do 
    if [ "$g" != '*.psf' ]
    then
        echo Linking ${g%.psf}.psf in aarch64 codebase ...;
        aarch64-linux-gnu-ld -r -b binary -o ${g%.psf}_PSF.o ${g%.psf}.psf; 
    fi
done
for j in *.json; do
    if [ "$j" != '*.json' ]
    then
        echo Linking ${j%.json}.json in aarch64 codebase ...;
        aarch64-linux-gnu-ld -r -b binary -o ${j%.json}_JSON.o ${j%.json}.json; 
    fi
done
for h in *.asm ; do
    if [ "$h" != '*.asm' ]
    then
        echo Assembling ${h%.asm}.asm ... in aarch64 codebase ...;
        aarch64-linux-gnu-as ${h%.asm}.asm -mcpu=cortex-a53 -o ${h%.asm}_ASM.o
    fi
done
