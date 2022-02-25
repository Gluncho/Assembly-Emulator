# Documentation

## Language used

Read about the simplified RISC assembly instructions [here](https://github.com/Gluncho/Assembly-Emulator/blob/main/12-Computer-Architecture.pdf)

## What's different?
There are few minor differences between the language described above and the language I used for this project.
The following project:
* Does not support data conversions yet (it only works with integers, not floating numbers).
* Does not need commas when using the branch, seperating tokens with spaces is enough. For example, you must replace ```BGE R1,R2,PC+12``` with something like ```BGE R1 R2 PC +12```
* Currently uses the keyword `M` instead of `Mem`. But you change those kind of preferences in [keywords.h](https://github.com/Gluncho/Assembly-Emulator/blob/main/keywords.h) file and everything will run perfectly.
* Is mostly space-friendly. Which means that you can add extra spaces in tests almost everywhere, for example, ```R1=  M[       4+ SP]``` and ```R1=M[SP+4]``` would both be valid instructions

## What's new?
* Bitwise operations ( ```&```, ```|``` ) are added, among with modular ```%``` operation. Note that you can basically tell the program what operations it can use just by editing ```ops[]``` array in [keywords.h](https://github.com/Gluncho/Assembly-Emulator/blob/main/keywords.h) file.
* Some built-in functions are added. Those functions currently are ```printStr, printf, strlen```.
```
strlen - takes char* as a parameter, and returns an integer - length of the respective string.
printStr - takes char* as a parameter, and prints the respective string.
printf - takes an integer as a parameter, and prints the integer itself.
```