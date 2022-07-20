#!/bin/bash

CDIR=$PWD
PACKAGE_SET=""
PACKAGE_SET_INFO=""
PACKAGE_SET_INFO1=""
PACKAGE_SET_INFO2=""
PACKAGE_SET_QEMU=0
PACKAGE_SET_TOOL=0
NO_PACKAGES=0

while getopts 'qbvh' SHELLARGUMENT; do
	case "$SHELLARGUMENT" in
		q)
			echo "! QEMU will be installed."
			PACKAGE_SET="qemu-system-x86-64 qemu-system-aarch64 ${PACKAGE_SET}"
			PACKAGE_SET_INFO1="QEMU (x86_64 and aarch64 VM packages)"
			PACKAGE_SET_QEMU=1
			;;
		b)
			echo "! Build Toolchain will be installed."
			PACKAGE_SET="${PACKAGE_SET_INFO} gcc iat make nasm build-essential binutils-aarch64-linux-gnu gcc-aarch64-linux-gnu"
			PACKAGE_SET_INFO2="Build Toolchain (x86_64 and aarch64)"
			PACKAGE_SET_TOOL=1
			;;
		h)
			echo "? Usage: setup.sh -qbvh"
			echo "? Options:"
			echo "-q => Install QEMU"
			echo "-b => Install Build Toolchain"
			echo "-h => Show help"
			echo "-v => Show version"
			exit 1
			;;
		v)
			echo "+ Tunnel OS Setup Script"
			echo "$ Version: 1.2"
			exit 0
			;;
		?)
			echo "? Usage: setup.sh -qbvh"
			echo "? Options:"
			echo "-q => Install QEMU"
			echo "-b => Install Build Toolchain"
			echo "-h => Show help"
			echo "-v => Show version"
			exit 1
			;;
	esac
done
shift "$(($OPTIND -1))"

if [ "$EUID" -ne 0 ]
then 
	echo "Setup command should be runned as root user"
	exit 1
fi

if [ $PACKAGE_SET_TOOL -eq 1] && [ $PACKAGE_SET_QEMU -eq 1 ]
then
	PACKAGE_SET_INFO="* Downloading ${PACKAGE_SET_INFO2} and ${PACKAGE_SET_INFO1}..."
fi
if [ $PACKAGE_SET_TOOL -eq 1] && [ $PACKAGE_SET_QEMU -eq 0 ] 
then
	PACKAGE_SET_INFO="* Downloading ${PACKAGE_SET_INFO2}"
fi
if [ $PACKAGE_SET_TOOL -eq 0] && [ $PACKAGE_SET_QEMU -eq 1 ] 
then
	PACKAGE_SET_INFO="* Downloading ${PACKAGE_SET_INFO1}"
fi
if [ $PACKAGE_SET_TOOL -eq 0] && [ $PACKAGE_SET_QEMU -eq 0 ] 
then
	NO_PACKAGES=1
fi

rm -f /usr/include/stdarg.h*
rm -f /usr/include/stdarg.h

if [ $NO_PACKAGES -eq 0]
then
echo $PACKAGE_SET_INFO
apt-get -qq install -o=Dpkg::Use-Pty=0 $PACKAGE_SET 1> /dev/null
fi

cd /usr/include
echo "* Downloading stdarg.h"
wget https://sites.uclouvain.be/SystInfo/usr/include/stdarg.h -q

cd $CDIR
echo "** Everything is set up to compile this code and execute system on supported targets!"
