cp compile.sh api/
cp compile.sh base/
cp compile.sh cJSON/
cp compile.sh drivers/
cp compile.sh etc/
cp compile.sh fonts/
cp compile.sh software/
cp compile.sh std/
cp compile.sh tunnelconfig/

cd api
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../base
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../cJSON
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../drivers
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../etc
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../fonts
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../software
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../std
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ../tunnelconfig
bash compile.sh
rm compile.sh
cp *.o ../ 2> /dev/null
rm *.o -r 2> /dev/null
cd ..