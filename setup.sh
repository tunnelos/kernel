#!/bin/bash

if [ "$EUID" -ne 0 ]
	then echo "Setup command should be runned as root user"
	exit
fi

apt install gcc iat make nasm -y
cd /usr/include
rm -f stdarg.h*
rm -f stdarg.h
wget https://sites.uclouvain.be/SystInfo/usr/include/stdarg.h

echo "Everything is set up to compile this code"
