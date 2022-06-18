cp compile.sh api/
cp compile.sh base/
cp compile.sh cJSON/
cp compile.sh drivers/
cp compile.sh etc/
cp compile.sh fonts/
cp compile.sh software/
cp compile.sh std/

cd api
bash compile.sh
rm compile.sh
cp *.o ../
cd ../base
bash compile.sh
rm compile.sh
cp *.o ../
cd ../cJSON
bash compile.sh
rm compile.sh
cp *.o ../
cd ../drivers
bash compile.sh
rm compile.sh
cp *.o ../
cd ../etc
bash compile.sh
rm compile.sh
cp *.o ../
cd ../fonts
bash compile.sh
rm compile.sh
cp *.o ../
cd ../software
bash compile.sh
rm compile.sh
cp *.o ../
cd ../std
bash compile.sh
rm compile.sh
cp *.o ../
cd ../
bash compile.sh
