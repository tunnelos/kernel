tasks=[]
taskI=0

tmpPWD=$PWD

CompileAPI() {
	cd ${tmpPWD}/api
	bash compile.sh
	rm compile.sh
	cp *.o ../ 2> /dev/null
	rm *.o -r 2> /dev/null
}
CompileBase() {
    cd ${tmpPWD}/base
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompilecJSON() {
    cd ${tmpPWD}/cJSON
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileDrivers() {
    cd ${tmpPWD}/drivers
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileETC() {
    cd ${tmpPWD}/etc
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileFonts() {
    cd ${tmpPWD}/fonts
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileSoftware() {
    cd ${tmpPWD}/software
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileSTD() {
    cd ${tmpPWD}/std
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}
CompileTunnelConfig() {
    cd ${tmpPWD}/tunnelconfig
    bash compile.sh
    rm compile.sh
    cp *.o ../ 2> /dev/null
    rm *.o -r 2> /dev/null
}

cp compile.sh api/
cp compile.sh base/
# cp compile.sh cJSON/
cp compile.sh drivers/
cp compile.sh etc/
cp compile.sh fonts/
cp compile.sh software/
cp compile.sh std/
cp compile.sh tunnelconfig/

CompileAPI &
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileBase &
tasks[${taskI}]=$!
taskI=$((taskI+1))
# CompilecJSON &
# tasks[${taskI}]=$!
# taskI=$((taskI+1))
CompileDrivers &
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileETC &
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileFonts
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileSoftware &
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileSTD &
tasks[${taskI}]=$!
taskI=$((taskI+1))
CompileTunnelConfig &
tasks[${taskI}]=$!
taskI=$((taskI+1))

for task in ${tasks[*]}; do
    if [ "$task" != '[]' ]
    then
         wait $task
    fi
done

echo Final linking ...