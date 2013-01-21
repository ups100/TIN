#!/bin/sh

if [ ! -d ./build ] 
then 
	mkdir build
	mkdir build/lib
	mkdir build/bin 
fi

cp ./QtDynamicLibs/* ./build/lib
cp help.txt ./build/bin

cd build
cmake ../code

