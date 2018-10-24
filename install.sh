#!/bin/sh

sudo cp build/libCrioLinux.so.0.1.0 /usr/local/lib/.
sudo ln -s /usr/local/lib/libCrioLinux.so.0.1.0 /usr/local/lib/libCrioLinux.so.0
sudo ln -s /usr/local/lib/libCrioLinux.so.0 /usr/local/lib/libCrioLinux.so

sudo cp include/CrioLinux.h /usr/local/include/.
