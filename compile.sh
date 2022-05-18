CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -mno-red-zone -Wno-shift-count-overflow -g -I./bootboot/dist/ -I./include -I/usr/include"

for f in *.c ; do 
    echo Compiling ${f%.c}.c ...;
    gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o; 
done
for g in fonts/*.psf ; do 
    echo Linking ${g%.psf}.psf ...;
    ld -r -b binary -o ${g%.psf}.o ${g%.psf}.psf; 
done
for j in tunnelconfig/*.json; do
    echo Linking ${j%.json}.json;
    ld -r -b binary -o ${j%.json}.o ${j%.json}.json; 
done
for h in *.asm ; do
    echo Assembling ${h%.asm}.asm ...;
    nasm -felf64 ${h%.asm}.asm -o ${h%.asm}a.o; 
done