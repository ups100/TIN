#!/bin/sh

if [ ! -d ./build ] 
then 
	mkdir build
	mkdir build/lib
	mkdir build/bin 
fi

cp ./QtDynamicLibs/* ./build/lib

cd build
cmake ../code
cp help.txt ./build/bin
