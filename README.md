# CoSimulation
CoSimulation Project

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
