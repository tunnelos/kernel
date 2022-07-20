CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -mno-red-zone -Wno-shift-count-overflow -I./bootboot/dist/ -I./include -I/usr/include"

for f in *.c ; do
    if [ "$f" != '*.c' ]
    then
        echo Compiling ${f%.c}.c in x86_64 codebase ...;
        gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o; 
    fi
done
for g in *.psf ; do 
    if [ "$g" != '*.psf' ]
    then
        echo Linking ${g%.psf}.psf in x86_64 codebase ...;
        ld -r -b binary -o ${g%.psf}_PSF.o ${g%.psf}.psf; 
    fi
done
for j in *.json; do
    if [ "$j" != '*.json' ]
    then
        echo Linking ${j%.json}.json in x86_64 codebase ...;
        ld -r -b binary -o ${j%.json}_JSON.o ${j%.json}.json; 
    fi
done
for h in *.asm ; do
    if [ "$h" != '*.asm' ]
    then
        echo Assembling ${h%.asm}.asm in x86_64 codebase ...;
        nasm -felf64 ${h%.asm}.asm -o ${h%.asm}_ASM.o;
    fi
done
