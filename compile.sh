CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -mno-red-zone -I./bootboot/dist/ -I./include -I/usr/include"

for f in *.c ; do gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o ; done
for g in fonts/*.psf ; do ld -r -b binary -o ${g%.psf}.o ${g%.psf}.psf; done