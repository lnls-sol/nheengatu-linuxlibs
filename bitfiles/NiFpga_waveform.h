/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_waveform_h__
#define __NiFpga_waveform_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_waveform_Bitfile;
 */
#define NiFpga_waveform_Bitfile "NiFpga_waveform.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_waveform_Signature = "100FDCC718F323065590BB9FA972CA54";

typedef enum
{
   NiFpga_waveform_IndicatorArrayU32_waveform_u32 = 0x18004,
} NiFpga_waveform_IndicatorArrayU32;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU32Size_waveform_u32 = 3,
} NiFpga_waveform_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU64_waveform_u64 = 0x18008,
} NiFpga_waveform_IndicatorArrayU64;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU64Size_waveform_u64 = 3,
} NiFpga_waveform_IndicatorArrayU64Size;

typedef enum
{
   NiFpga_waveform_IndicatorArraySgl_waveform_sgl = 0x1800C,
} NiFpga_waveform_IndicatorArraySgl;

typedef enum
{
   NiFpga_waveform_IndicatorArraySglSize_waveform_sgl = 3,
} NiFpga_waveform_IndicatorArraySglSize;

typedef enum
{
   NiFpga_waveform_ControlArrayU32_waveform_u32 = 0x18000,
} NiFpga_waveform_ControlArrayU32;

typedef enum
{
   NiFpga_waveform_ControlArrayU32Size_waveform_u32 = 3,
} NiFpga_waveform_ControlArrayU32Size;

typedef enum
{
   NiFpga_waveform_ControlArrayU64_waveform_u64 = 0x18010,
} NiFpga_waveform_ControlArrayU64;

typedef enum
{
   NiFpga_waveform_ControlArrayU64Size_waveform_u64 = 3,
} NiFpga_waveform_ControlArrayU64Size;

typedef enum
{
   NiFpga_waveform_ControlArraySgl_waveform_sgl = 0x18014,
} NiFpga_waveform_ControlArraySgl;

typedef enum
{
   NiFpga_waveform_ControlArraySglSize_waveform_sgl = 3,
} NiFpga_waveform_ControlArraySglSize;

#endif
