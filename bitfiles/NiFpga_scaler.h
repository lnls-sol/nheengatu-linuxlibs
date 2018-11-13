/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_scaler64_example_h__
#define __NiFpga_scaler64_example_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_scaler64_example_Bitfile;
 */
#define NiFpga_scaler64_example_Bitfile "NiFpga_scaler64_example.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_scaler64_example_Signature = "FA92C3A36F91F3D6C1873A65C040D594";

typedef enum
{
   NiFpga_scaler64_example_IndicatorBool_Done_Scaler = 0x1800E,
} NiFpga_scaler64_example_IndicatorBool;

typedef enum
{
   NiFpga_scaler64_example_IndicatorU64_Pulse_Scaler = 0x18000,
} NiFpga_scaler64_example_IndicatorU64;

typedef enum
{
   NiFpga_scaler64_example_ControlBool_Enable_Scaler = 0x1801A,
   NiFpga_scaler64_example_ControlBool_OneShot_Scaler = 0x18006,
} NiFpga_scaler64_example_ControlBool;

typedef enum
{
   NiFpga_scaler64_example_IndicatorArrayU32_Counter_Scaler = 0x18008,
} NiFpga_scaler64_example_IndicatorArrayU32;

typedef enum
{
   NiFpga_scaler64_example_IndicatorArrayU32Size_Counter_Scaler = 64,
} NiFpga_scaler64_example_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_scaler64_example_ControlArrayBool_Gate_Scaler = 0x18014,
} NiFpga_scaler64_example_ControlArrayBool;

typedef enum
{
   NiFpga_scaler64_example_ControlArrayBoolSize_Gate_Scaler = 64,
} NiFpga_scaler64_example_ControlArrayBoolSize;

typedef enum
{
   NiFpga_scaler64_example_ControlArrayU32_PresetValues_Scaler = 0x18010,
} NiFpga_scaler64_example_ControlArrayU32;

typedef enum
{
   NiFpga_scaler64_example_ControlArrayU32Size_PresetValues_Scaler = 64,
} NiFpga_scaler64_example_ControlArrayU32Size;

#endif
