#!/bin/sh

if [ ! -d ./build ] 
then 
	mkdir build
	mkdir build/lib 
fi

cp ./QtDynamicLibs/* ./build/lib

cd build
cmake ../code
