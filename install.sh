#!/bin/sh

cp -f build/libCrioLinux.so.0.1.0 /usr/local/lib/.
ln -sf /usr/local/lib/libCrioLinux.so.0.1.0 /usr/local/lib/libCrioLinux.so.0
ln -sf /usr/local/lib/libCrioLinux.so.0 /usr/local/lib/libCrioLinux.so

cp -f include/CrioLinux.h /usr/local/include/.
ls -alh /usr/local/include/CrioLinux.h /usr/local/lib/libCrioLinux.so*
ldconfig
echo "CrioLinux Library installation complete"
