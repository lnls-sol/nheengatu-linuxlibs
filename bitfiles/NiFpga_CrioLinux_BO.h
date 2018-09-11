/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_CrioLinux_h__
#define __NiFpga_CrioLinux_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_CrioLinux_Bitfile;
 */
#define NiFpga_CrioLinux_Bitfile "NiFpga_CrioLinux.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_CrioLinux_Signature = "63D3E6B13CF2FF5CD0BCE2134F6EEB4D";

typedef enum
{
   NiFpga_CrioLinux_ControlBool_BO0 = 0x18002,
   NiFpga_CrioLinux_ControlBool_BO1 = 0x1807E,
   NiFpga_CrioLinux_ControlBool_BO10 = 0x1805A,
   NiFpga_CrioLinux_ControlBool_BO11 = 0x18056,
   NiFpga_CrioLinux_ControlBool_BO12 = 0x18052,
   NiFpga_CrioLinux_ControlBool_BO13 = 0x1804E,
   NiFpga_CrioLinux_ControlBool_BO14 = 0x1804A,
   NiFpga_CrioLinux_ControlBool_BO15 = 0x1800A,
   NiFpga_CrioLinux_ControlBool_BO16 = 0x18046,
   NiFpga_CrioLinux_ControlBool_BO17 = 0x18042,
   NiFpga_CrioLinux_ControlBool_BO18 = 0x1803E,
   NiFpga_CrioLinux_ControlBool_BO19 = 0x1803A,
   NiFpga_CrioLinux_ControlBool_BO2 = 0x1807A,
   NiFpga_CrioLinux_ControlBool_BO20 = 0x18036,
   NiFpga_CrioLinux_ControlBool_BO21 = 0x18032,
   NiFpga_CrioLinux_ControlBool_BO22 = 0x1802E,
   NiFpga_CrioLinux_ControlBool_BO23 = 0x1802A,
   NiFpga_CrioLinux_ControlBool_BO24 = 0x18026,
   NiFpga_CrioLinux_ControlBool_BO25 = 0x18022,
   NiFpga_CrioLinux_ControlBool_BO26 = 0x1801E,
   NiFpga_CrioLinux_ControlBool_BO27 = 0x1801A,
   NiFpga_CrioLinux_ControlBool_BO28 = 0x18016,
   NiFpga_CrioLinux_ControlBool_BO29 = 0x18012,
   NiFpga_CrioLinux_ControlBool_BO3 = 0x18076,
   NiFpga_CrioLinux_ControlBool_BO30 = 0x1800E,
   NiFpga_CrioLinux_ControlBool_BO31 = 0x18006,
   NiFpga_CrioLinux_ControlBool_BO4 = 0x18072,
   NiFpga_CrioLinux_ControlBool_BO5 = 0x1806E,
   NiFpga_CrioLinux_ControlBool_BO6 = 0x1806A,
   NiFpga_CrioLinux_ControlBool_BO7 = 0x18066,
   NiFpga_CrioLinux_ControlBool_BO8 = 0x18062,
   NiFpga_CrioLinux_ControlBool_BO9 = 0x1805E,
} NiFpga_CrioLinux_ControlBool;

#endif
