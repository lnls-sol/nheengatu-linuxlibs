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
static const char* const NiFpga_waveform_Signature = "071ABA139A0C89D5C7E4051E2DB7F220";

typedef enum
{
   NiFpga_waveform_IndicatorArrayI8_waveform_i8 = 0x18038,
} NiFpga_waveform_IndicatorArrayI8;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI8Size_waveform_i8 = 3,
} NiFpga_waveform_IndicatorArrayI8Size;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU8_waveform_u8 = 0x18020,
} NiFpga_waveform_IndicatorArrayU8;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU8Size_waveform_u8 = 3,
} NiFpga_waveform_IndicatorArrayU8Size;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI16_waveform_i16 = 0x18028,
} NiFpga_waveform_IndicatorArrayI16;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI16Size_waveform_i16 = 3,
} NiFpga_waveform_IndicatorArrayI16Size;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU16_waveform_u16 = 0x18018,
} NiFpga_waveform_IndicatorArrayU16;

typedef enum
{
   NiFpga_waveform_IndicatorArrayU16Size_waveform_u16 = 3,
} NiFpga_waveform_IndicatorArrayU16Size;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI32_waveform_i32 = 0x18030,
} NiFpga_waveform_IndicatorArrayI32;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI32Size_waveform_i32 = 3,
} NiFpga_waveform_IndicatorArrayI32Size;

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
   NiFpga_waveform_IndicatorArrayI64_waveform_i64 = 0x18040,
} NiFpga_waveform_IndicatorArrayI64;

typedef enum
{
   NiFpga_waveform_IndicatorArrayI64Size_waveform_i64 = 3,
} NiFpga_waveform_IndicatorArrayI64Size;

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
   NiFpga_waveform_ControlArrayI8_waveform_i8 = 0x1803C,
} NiFpga_waveform_ControlArrayI8;

typedef enum
{
   NiFpga_waveform_ControlArrayI8Size_waveform_i8 = 3,
} NiFpga_waveform_ControlArrayI8Size;

typedef enum
{
   NiFpga_waveform_ControlArrayU8_waveform_u8 = 0x18024,
} NiFpga_waveform_ControlArrayU8;

typedef enum
{
   NiFpga_waveform_ControlArrayU8Size_waveform_u8 = 3,
} NiFpga_waveform_ControlArrayU8Size;

typedef enum
{
   NiFpga_waveform_ControlArrayI16_waveform_i16 = 0x1802C,
} NiFpga_waveform_ControlArrayI16;

typedef enum
{
   NiFpga_waveform_ControlArrayI16Size_waveform_i16 = 3,
} NiFpga_waveform_ControlArrayI16Size;

typedef enum
{
   NiFpga_waveform_ControlArrayU16_waveform_u16 = 0x1801C,
} NiFpga_waveform_ControlArrayU16;

typedef enum
{
   NiFpga_waveform_ControlArrayU16Size_waveform_u16 = 3,
} NiFpga_waveform_ControlArrayU16Size;

typedef enum
{
   NiFpga_waveform_ControlArrayI32_waveform_i32 = 0x18034,
} NiFpga_waveform_ControlArrayI32;

typedef enum
{
   NiFpga_waveform_ControlArrayI32Size_waveform_i32 = 3,
} NiFpga_waveform_ControlArrayI32Size;

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
   NiFpga_waveform_ControlArrayI64_waveform_i64 = 0x18044,
} NiFpga_waveform_ControlArrayI64;

typedef enum
{
   NiFpga_waveform_ControlArrayI64Size_waveform_i64 = 3,
} NiFpga_waveform_ControlArrayI64Size;

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
