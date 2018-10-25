# crio-linux-libs: Configurable wrapper for National Instruments C API library

This project packages functions for configuring the FPGA CRIO, reading and 
writing binary and analog variables from and to the FPGA and the labview RT.

## Usage:

Include the CrioLinux.h in your C code

	$ g++ -std=c++11 someprogram.cc -o someprogram -lCrioLinux

Make sure the library is in your library search path (or explicitly specify
link time and run time search directories).

## Build:

	$ ./build.sh

## Install:

	$ sudo ./install.sh
