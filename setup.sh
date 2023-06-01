#!/bin/bash

CDIR=$PWD
PACKAGE_SET=""
PACKAGE_SET_INFO=""
PACKAGE_SET_INFO1=""
PACKAGE_SET_INFO2=""
PACKAGE_SET_QEMU=0
PACKAGE_SET_TOOL=0
NO_PACKAGES=0
VERBOSE=0

while getopts 'qbvhr' SHELLARGUMENT; do
	case "$SHELLARGUMENT" in
		q)
			echo "! QEMU will be installed."
			PACKAGE_SET="qemu-system-x86 qemu-system-aarch64 ${PACKAGE_SET}"
			PACKAGE_SET_INFO1="QEMU (x86_64 and aarch64 VM packages)"
			PACKAGE_SET_QEMU=1
			;;
		b)
			echo "! Build Toolchain will be installed."
			PACKAGE_SET="${PACKAGE_SET_INFO} ${PACKAGE_SET} gcc iat make nasm build-essential binutils-aarch64-linux-gnu gcc-11-aarch64-linux-gnu zip"
			PACKAGE_SET_INFO2="Build Toolchain (x86_64 and aarch64)"
			PACKAGE_SET_TOOL=1
			;;
		h)
			echo "? Usage: setup.sh -qbvh"
			echo "? Options:"
			echo " -q => Install QEMU"
			echo " -b => Install Build Toolchain"
			echo " -h => Show help"
			echo " -v => Show version"
			echo " -r => Verbose output"
			exit 0
			;;
		v)
			echo "+ Tunnel OS Setup Script"
			echo "$ Version: 1.34"
			exit 0
			;;
		r)
			VERBOSE=1
			;;
		?)
			echo "? Usage: setup.sh -qbvh"
			echo "? Options:"
			echo " -q => Install QEMU"
			echo " -b => Install Build Toolchain"
			echo " -h => Show help"
			echo " -v => Show version"
			echo " -r => Verbose output"
			exit 1
			;;
	esac
done
shift "$(($OPTIND -1))"

if [ "$EUID" -ne 0 ]
then 
	echo "! Setup command should be runned as root user."
	exit 1
fi

if [ $PACKAGE_SET_TOOL -eq 1 ] && [ $PACKAGE_SET_QEMU -eq 1 ]
then
	PACKAGE_SET_INFO="@ Downloading ${PACKAGE_SET_INFO2} and ${PACKAGE_SET_INFO1}..."
fi
if [ $PACKAGE_SET_TOOL -eq 1 ] && [ $PACKAGE_SET_QEMU -eq 0 ] 
then
	PACKAGE_SET_INFO="@ Downloading ${PACKAGE_SET_INFO2}"
fi
if [ $PACKAGE_SET_TOOL -eq 0 ] && [ $PACKAGE_SET_QEMU -eq 1 ] 
then
	PACKAGE_SET_INFO="@ Downloading ${PACKAGE_SET_INFO1}"
fi
if [ $PACKAGE_SET_TOOL -eq 0 ] && [ $PACKAGE_SET_QEMU -eq 0 ] 
then
	NO_PACKAGES=1
fi

if [ $VERBOSE -eq 1 ]
then
rm -vf /usr/include/stdarg.h /usr/include/cpuid.h
else
rm -f /usr/include/stdarg.h /usr/include/cpuid.h
fi

if [ $NO_PACKAGES -eq 0 ]
then
	echo $PACKAGE_SET_INFO;
	if [ $VERBOSE -eq 1 ]
	then
		apt-get install $PACKAGE_SET -y;
	else
		apt-get -qq install -o=Dpkg::Use-Pty=0 $PACKAGE_SET -y 1> /dev/null;
	fi
fi

cd /usr/include
echo "* Downloading stdarg.h"
if [ $VERBOSE -eq 1 ]
then
	wget https://sites.uclouvain.be/SystInfo/usr/include/stdarg.h
else
	wget https://sites.uclouvain.be/SystInfo/usr/include/stdarg.h -q
fi
echo "* Downloading cpuid.h"
if [ $VERBOSE -eq 1 ]
then
	wget https://sites.uclouvain.be/SystInfo/usr/include/cpuid.h
else
	wget https://sites.uclouvain.be/SystInfo/usr/include/cpuid.h -q
fi

cd $CDIR
echo "** Everything is set up to compile this code and execute system on supported targets!"
