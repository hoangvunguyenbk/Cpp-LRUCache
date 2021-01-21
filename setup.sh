#!/bin/bash

sudo apt-get update

#build-essentil
sudo apt-get install -y g++

#cmake
sudo apt-get install -y cmake

#gtest
sudo apt-get install -y libgtest-dev

cd /usr/src/gtest

sudo cmake CMakeLists.txt
sudo make

FILE=./libgtest.a
if [ -f $FILE ]; then
	sudo cp *a /usr/lib
else
	sudo cp ./lib/libgtest*.a /usr/lib
fi

#boost lib
sudo apt-get install -y libboost-all-dev
