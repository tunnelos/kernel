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
cp *.o ../
rm *.o -r
cd ../base
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../cJSON
bash compile.sh
rm compile.sh
cp *.o ../
cd ../drivers
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../etc
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../fonts
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../software
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../std
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ../tunnelconfig
bash compile.sh
rm compile.sh
cp *.o ../
rm *.o -r
cd ..
pwd