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
static const char* const NiFpga_CrioLinux_Signature = "43885B73727B7968270BCC1CA8926DC1";

typedef enum
{
   NiFpga_CrioLinux_IndicatorSgl_AI0 = 0x18004,
   NiFpga_CrioLinux_IndicatorSgl_AI1 = 0x18008,
   NiFpga_CrioLinux_IndicatorSgl_AI2 = 0x1800C,
   NiFpga_CrioLinux_IndicatorSgl_AI3 = 0x18000,
} NiFpga_CrioLinux_IndicatorSgl;

#endif
