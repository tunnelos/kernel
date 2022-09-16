CFLAGS="-Wall -fpic -ffreestanding -fno-stack-protector -nostdinc -nostdlib -mno-red-zone -Wno-shift-count-overflow -g -I./bootboot/dist/ -I./include -I/usr/include"

tasks=[]
taskI=0

for f in *.c ; do
    if [ "$f" != '*.c' ]
    then
        echo Compiling ${f%.c}.c in x86_64 codebase ...;
        gcc ${CFLAGS} -c ${f%.c}.c -o "${f%.c}".o &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done
for t in *.cpp ; do
    if [ "$t" != '*.cpp' ]
    then
        echo Compiling ${t%.cpp}.cpp in x86_64 codebase ...;
        g++ ${CFLAGS} -c ${t%.cpp}.cpp -o "${t%.cpp}".o &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done
for g in *.psf ; do 
    if [ "$g" != '*.psf' ]
    then
        echo Linking ${g%.psf}.psf in x86_64 codebase ...;
        ld -r -b binary -o ${g%.psf}_PSF.o ${g%.psf}.psf &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done
for j in *.json; do
    if [ "$j" != '*.json' ]
    then
        echo Linking ${j%.json}.json in x86_64 codebase ...;
        ld -r -b binary -o ${j%.json}_JSON.o ${j%.json}.json &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done
for h in *.asm ; do
    if [ "$h" != '*.asm' ]
    then
        echo Assembling ${h%.asm}.asm in x86_64 codebase ...;
        nasm -felf64 ${h%.asm}.asm -o ${h%.asm}_ASM.o &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done

for task in ${tasks[*]}; do
    if [ "$task" != '[]' ]
    then
        wait $task
    fi
done
