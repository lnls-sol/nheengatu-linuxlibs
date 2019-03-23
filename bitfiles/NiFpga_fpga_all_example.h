/*
 * Generated with the FPGA Interface C API Generator 18.0.0
 * for NI-RIO 18.0.0 or later.
 */

#ifndef __NiFpga_fpga_all_example_h__
#define __NiFpga_fpga_all_example_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1800
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_fpga_all_example_Bitfile;
 */
#define NiFpga_fpga_all_example_Bitfile "NiFpga_fpga_all_example.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_fpga_all_example_Signature = "98D458A8E57C226145D9BF228C21052D";

typedef enum
{
   NiFpga_fpga_all_example_IndicatorBool_Done_SCALER1 = 0x1800A,
   NiFpga_fpga_all_example_IndicatorBool_Done_SCALER2 = 0x1801A,
} NiFpga_fpga_all_example_IndicatorBool;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorU32_Looptime = 0x180F0,
} NiFpga_fpga_all_example_IndicatorU32;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorU64_BI = 0x18080,
} NiFpga_fpga_all_example_IndicatorU64;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI0 = 0x18070,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI1 = 0x18074,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI2 = 0x18078,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI3 = 0x1807C,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC0 = 0x18050,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC1 = 0x18054,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC2 = 0x18058,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC3 = 0x1805C,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI0 = 0x18040,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI1 = 0x18044,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI2 = 0x18048,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI3 = 0x1804C,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI0 = 0x1803C,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI1 = 0x18038,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI2 = 0x18034,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI3 = 0x18030,
} NiFpga_fpga_all_example_IndicatorSgl;

typedef enum
{
   NiFpga_fpga_all_example_ControlBool_Enable_SCALER1 = 0x18012,
   NiFpga_fpga_all_example_ControlBool_Enable_SCALER2 = 0x18022,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO0 = 0x18092,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO1 = 0x1808E,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO2 = 0x1808A,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO3 = 0x18086,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO0 = 0x180A2,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO1 = 0x1809E,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO2 = 0x1809A,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO3 = 0x18096,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO16 = 0x180A6,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO17 = 0x180AA,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO18 = 0x180AE,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO19 = 0x180B2,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO20 = 0x180B6,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO21 = 0x180BA,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO22 = 0x180BE,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO23 = 0x180C2,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO24 = 0x180C6,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO25 = 0x180CA,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO26 = 0x180CE,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO27 = 0x180D2,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO28 = 0x180D6,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO29 = 0x180DA,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO30 = 0x180DE,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO31 = 0x180E2,
   NiFpga_fpga_all_example_ControlBool_OneShot_SCALER1 = 0x18016,
   NiFpga_fpga_all_example_ControlBool_OneShot_SCALER2 = 0x18026,
   NiFpga_fpga_all_example_ControlBool_sample_SCALER2 = 0x180EA,
} NiFpga_fpga_all_example_ControlBool;

typedef enum
{
   NiFpga_fpga_all_example_ControlSgl_Mod5AO0 = 0x1806C,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO1 = 0x18068,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO2 = 0x18064,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO3 = 0x18060,
} NiFpga_fpga_all_example_ControlSgl;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorArrayU32_AnalogInputs_SCALER2 = 0x180EC,
   NiFpga_fpga_all_example_IndicatorArrayU32_Counter_SCALER1 = 0x18004,
   NiFpga_fpga_all_example_IndicatorArrayU32_Counter_SCALER2 = 0x1801C,
} NiFpga_fpga_all_example_IndicatorArrayU32;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorArrayU32Size_AnalogInputs_SCALER2 = 1,
   NiFpga_fpga_all_example_IndicatorArrayU32Size_Counter_SCALER1 = 2,
   NiFpga_fpga_all_example_IndicatorArrayU32Size_Counter_SCALER2 = 2,
} NiFpga_fpga_all_example_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayBool_Gate_SCALER1 = 0x1800E,
   NiFpga_fpga_all_example_ControlArrayBool_Gate_SCALER2 = 0x1802A,
   NiFpga_fpga_all_example_ControlArrayBool_Pulse_SCALER1 = 0x180E6,
} NiFpga_fpga_all_example_ControlArrayBool;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayBoolSize_Gate_SCALER1 = 2,
   NiFpga_fpga_all_example_ControlArrayBoolSize_Gate_SCALER2 = 2,
   NiFpga_fpga_all_example_ControlArrayBoolSize_Pulse_SCALER1 = 1,
} NiFpga_fpga_all_example_ControlArrayBoolSize;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayU32_PresetValues_SCALER1 = 0x18000,
   NiFpga_fpga_all_example_ControlArrayU32_PresetValues_SCALER2 = 0x1802C,
} NiFpga_fpga_all_example_ControlArrayU32;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayU32Size_PresetValues_SCALER1 = 2,
   NiFpga_fpga_all_example_ControlArrayU32Size_PresetValues_SCALER2 = 2,
} NiFpga_fpga_all_example_ControlArrayU32Size;

#endif
