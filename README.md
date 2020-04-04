# Nheengatu Linux Libraries: Configurable wrapper for National Instruments C API library

This project provides a layer of communication between the NI FPGA functions/LabviewVI
and the EPICS device support. It abstracts all complexities generated from identifying 
whether the variable accessed is from the LabviewRT or the FPGA through a configuration
file that is provided upon initialization. More information can be found [here](https://github.com/lnls-sol/project-nheengatu/tree/master/docs).


## Usage:

Include the CrioLinux.h in your C code

	$ g++ -std=c++11 someprogram.cc -o someprogram -lCrioLinux

Make sure the library is in your library search path (or explicitly specify
link time and run time search directories).

## Build:

	$ ./build.sh

## Install:

	$ sudo ./install.sh

## Debugging with GDB

Edit the CMakeLists.txt, and exchange Release with Debug, and compile again.
