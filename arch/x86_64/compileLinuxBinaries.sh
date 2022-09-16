CFLAGS=""

tasks=[]
taskI=0

for f in *.c ; do
    if [ "$f" != '*.c' ]
    then
        echo Compiling ${f%.c}.c in x86_64 codebase ...;
        gcc ${CFLAGS} ${f%.c}.c -o "${f%.c}".bin &
        tasks[${taskI}]=$!;
        taskI=$((taskI+1));
    fi
done
for t in *.cpp ; do
    if [ "$t" != '*.cpp' ]
    then
        echo Compiling ${t%.cpp}.cpp in x86_64 codebase ...;
        g++ ${CFLAGS} ${t%.cpp}.cpp -o "${t%.cpp}".bin &
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
