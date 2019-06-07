/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_fixedPoint_h__
#define __NiFpga_fixedPoint_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_fixedPoint_Bitfile;
 */
#define NiFpga_fixedPoint_Bitfile "NiFpga_fixedPoint.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_fixedPoint_Signature = "FAC823B8F3E033E6495A2A3F08A15476";

typedef enum
{
   NiFpga_fixedPoint_IndicatorU64_aifxp1 = 0x18004,
   NiFpga_fixedPoint_IndicatorU64_aifxp2 = 0x1800C,
   NiFpga_fixedPoint_IndicatorU64_aifxp3 = 0x18014,
   NiFpga_fixedPoint_IndicatorU64_aifxp4 = 0x1801C,
   NiFpga_fixedPoint_IndicatorU64_aifxp5 = 0x18024,
   NiFpga_fixedPoint_IndicatorU64_aifxp6 = 0x1802C,
} NiFpga_fixedPoint_IndicatorU64;

typedef enum
{
   NiFpga_fixedPoint_ControlU64_u641 = 0x18030,
   NiFpga_fixedPoint_ControlU64_u642 = 0x18034,
   NiFpga_fixedPoint_ControlU64_u643 = 0x18038,
   NiFpga_fixedPoint_ControlU64_u644 = 0x1803C,
   NiFpga_fixedPoint_ControlU64_u645 = 0x18040,
   NiFpga_fixedPoint_ControlU64_u646 = 0x18044,
} NiFpga_fixedPoint_ControlU64;

#endif
