# CoSimulation
CoSimulation Project

The main idea of this project is create an example for advaced technic of software and hardware testing, knowed as co-simulation. For this test mechanism the driver needed to comunication between sw and hw is abstracted to inter communication process, for this specific case (Unix environment) the "pipe" command garantee the shared data flow.

Software written here uses C language and hardware is described by VHDL language. The hardware project is very simple, just a 8 bit full adder and the software goal is send two values ("A" and "B") to hardware execute the sum operation.

With this prove of concept, another complex interfaces can be abstracted, like USB, PCI, DMA, etc. I hope this project can speed up the functional tests for projects that uses software and synthesized hardware (FPGA) in embedded system area.

![Screenshot from 2019-09-04 22-01-23](https://user-images.githubusercontent.com/27175864/64291308-c1191500-cf5f-11e9-8a9c-f8f17c9dbcb5.png)

# Usage

## Building hardware

Run and Install
```
source /usr/share/Xilinx/14.5/ISE_DS/settings64.sh
```

### Executing hardware

## Building software
```
autoreconf -i
./configure
make
```

### Executing software

./Cosimulation

```
Cosimulation>help
Commands:
  verbose - Produce verbose output
  avalue A - Value A (A<=255) to sum operation
  bvalue B - Value B (B<=255) to sum operation
  sum - Execute "sum" command to get result for sum operation between A and B
  pipein - Pipe from simulated hardware
  pipeout - Pipe to simulated hardware
  help - Display help
  quit - Exit prompt command

Cosimulation>avalue 10
Cosimulation>bvalue 7
Cosimulation>sum
  Result = 17
  Carry = 0

Cosimulation>verbose
Toogle verbose mode: Activated 
Cosimulation>avalue 6
A Value:6
Cosimulation>bvalue 9
B Value:9
Cosimulation>sum
Sum operation executing on FPGA ...
  Result = 15
  Carry = 0
```
#Tests

## Pipe Bus Test

![PipeBus-Usage](https://user-images.githubusercontent.com/27175864/64924122-e831db00-d7d8-11e9-9609-fe2dc1ffda48.png)
