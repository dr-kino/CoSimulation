#!/bin/bash

FILE=build/TestPipeBus

if [ -f "$FILE" ]; then
	echo "Build was done..."
	rm -Rf build/bus_hw2sw.txt
	rm -Rf build/bus_sw2hw.txt
	mkfifo build/bus_hw2sw.txt
	mkfifo build/bus_sw2hw.txt

	cd build
	echo "Lauching TestPipeBus..."
	./TestPipeBus bus_hw2sw.txt bus_sw2hw.txt 
else
	echo "\"TestPipeBus\" executable not exist! Please, build test project"
fi
