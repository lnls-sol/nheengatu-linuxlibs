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
static const char* const NiFpga_CrioLinux_Signature = "BDBA267F01207E658C5F07DB845109FE";

typedef enum
{
   NiFpga_CrioLinux_ControlU32_AO13 = 0x18004,
} NiFpga_CrioLinux_ControlU32;

typedef enum
{
   NiFpga_CrioLinux_ControlSgl_AO0 = 0x18000,
   NiFpga_CrioLinux_ControlSgl_AO1 = 0x1800C,
   NiFpga_CrioLinux_ControlSgl_AO12 = 0x18008,
} NiFpga_CrioLinux_ControlSgl;

#endif
