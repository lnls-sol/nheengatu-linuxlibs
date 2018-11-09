/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_scaler_h__
#define __NiFpga_scaler_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_scaler_Bitfile;
 */
#define NiFpga_scaler_Bitfile "NiFpga_scaler.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_scaler_Signature = "2BFFA7116A1C06EF4760062F45B61CBB";

typedef enum
{
   NiFpga_scaler_IndicatorBool_Mod2DIO0 = 0x18002,
   NiFpga_scaler_IndicatorBool_Scaler0_Done = 0x18016,
} NiFpga_scaler_IndicatorBool;

typedef enum
{
   NiFpga_scaler_ControlBool_Scaler0_Enable = 0x1801A,
   NiFpga_scaler_ControlBool_Scaler0_OneShot = 0x1801E,
   NiFpga_scaler_ControlBool_Scaler0_Reset = 0x18006,
} NiFpga_scaler_ControlBool;

typedef enum
{
   NiFpga_scaler_IndicatorArrayU32_Scaler0_Counters = 0x1800C,
} NiFpga_scaler_IndicatorArrayU32;

typedef enum
{
   NiFpga_scaler_IndicatorArrayU32Size_Scaler0_Counters = 64,
} NiFpga_scaler_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_scaler_ControlArrayBool_Scaler0_Gate = 0x18008,
} NiFpga_scaler_ControlArrayBool;

typedef enum
{
   NiFpga_scaler_ControlArrayBoolSize_Scaler0_Gate = 64,
} NiFpga_scaler_ControlArrayBoolSize;

typedef enum
{
   NiFpga_scaler_ControlArrayU32_Scaler0_PresetValues = 0x18010,
} NiFpga_scaler_ControlArrayU32;

typedef enum
{
   NiFpga_scaler_ControlArrayU32Size_Scaler0_PresetValues = 64,
} NiFpga_scaler_ControlArrayU32Size;

#endif
