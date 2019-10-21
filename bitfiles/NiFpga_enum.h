/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_enum_h__
#define __NiFpga_enum_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_enum_Bitfile;
 */
#define NiFpga_enum_Bitfile "NiFpga_enum.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_enum_Signature = "4378E84899946051B7A1ADCFD8595A44";

typedef enum
{
   NiFpga_enum_IndicatorU16_MBBI = 0x18006,
} NiFpga_enum_IndicatorU16;

typedef enum
{
   NiFpga_enum_ControlU16_MBBO = 0x18002,
} NiFpga_enum_ControlU16;

#endif
