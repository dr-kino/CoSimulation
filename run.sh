#!/bin/bash

# Creating Co-Simulation pipes
cd cosim
rm -Rf pipe
mkdir -p pipe
mkfifo pipe/bus_hw2sw.txt
mkfifo pipe/bus_sw2hw.txt

# Go back to root folder
cd ../

# Run vhdl simulation
echo Launching VHDL simulation...
cd hw/xil/Adder
./fulladder -gui -tclbatch Adder.tcl -view wave_form.wcfg > /dev/null &

# Go back to root folder
cd ../../../sw

# Run software simulation
echo Launching software simulation...
./CoSimulation ../cosim/pipe/bus_hw2sw.txt ../cosim/pipe/bus_sw2hw.txt
