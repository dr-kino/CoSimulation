#!/bin/bash

# Run vhdl simulation
echo Launching VHDL simulation...
cd hw/xil/Adder
./fulladder -gui -tclbatch Adder.tcl -view wave_form.wcfg > /dev/null &

# Go back to root folder
cd ../../../
