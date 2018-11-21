/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_fpga_all_example_h__
#define __NiFpga_fpga_all_example_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
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
static const char* const NiFpga_fpga_all_example_Signature = "5AA54FF8107B38FE7588C1905492E54C";

typedef enum
{
   NiFpga_fpga_all_example_IndicatorBool_Done_Scaler_Analog = 0x18016,
   NiFpga_fpga_all_example_IndicatorBool_Done_Scaler_Digital = 0x18026,
} NiFpga_fpga_all_example_IndicatorBool;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorU32_Looptime = 0x18034,
} NiFpga_fpga_all_example_IndicatorU32;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorU64_BI = 0x180A0,
} NiFpga_fpga_all_example_IndicatorU64;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI0 = 0x180B0,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI1 = 0x180AC,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI2 = 0x180A8,
   NiFpga_fpga_all_example_IndicatorSgl_Mod4AI3 = 0x180A4,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC0 = 0x180D0,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC1 = 0x180CC,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC2 = 0x180C8,
   NiFpga_fpga_all_example_IndicatorSgl_Mod6TC3 = 0x180C4,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI0 = 0x180E0,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI1 = 0x180DC,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI2 = 0x180D8,
   NiFpga_fpga_all_example_IndicatorSgl_Mod7AI3 = 0x180D4,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI0 = 0x180E4,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI1 = 0x180E8,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI2 = 0x180EC,
   NiFpga_fpga_all_example_IndicatorSgl_Mod8AI3 = 0x180F0,
} NiFpga_fpga_all_example_IndicatorSgl;

typedef enum
{
   NiFpga_fpga_all_example_ControlBool_Enable_Scaler_Analog = 0x1800E,
   NiFpga_fpga_all_example_ControlBool_Enable_Scaler_Digital = 0x1801E,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO0 = 0x18092,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO1 = 0x18096,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO2 = 0x1809A,
   NiFpga_fpga_all_example_ControlBool_Mod1DIO3 = 0x1809E,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO0 = 0x18082,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO1 = 0x18086,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO2 = 0x1808A,
   NiFpga_fpga_all_example_ControlBool_Mod2DIO3 = 0x1808E,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO16 = 0x1807E,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO17 = 0x1807A,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO18 = 0x18076,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO19 = 0x18072,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO20 = 0x1806E,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO21 = 0x1806A,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO22 = 0x18066,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO23 = 0x18062,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO24 = 0x1805E,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO25 = 0x1805A,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO26 = 0x18056,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO27 = 0x18052,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO28 = 0x1804E,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO29 = 0x1804A,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO30 = 0x18046,
   NiFpga_fpga_all_example_ControlBool_Mod3DIO31 = 0x18042,
   NiFpga_fpga_all_example_ControlBool_OneShot_Scaler_Analog = 0x1800A,
   NiFpga_fpga_all_example_ControlBool_OneShot_Scaler_Digital = 0x1801A,
   NiFpga_fpga_all_example_ControlBool_sample = 0x18032,
} NiFpga_fpga_all_example_ControlBool;

typedef enum
{
   NiFpga_fpga_all_example_ControlSgl_Mod5AO0 = 0x180B4,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO1 = 0x180B8,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO2 = 0x180BC,
   NiFpga_fpga_all_example_ControlSgl_Mod5AO3 = 0x180C0,
} NiFpga_fpga_all_example_ControlSgl;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorArrayU32_AnalogInputs_Scaler_Analog = 0x18038,
   NiFpga_fpga_all_example_IndicatorArrayU32_Counter_Scaler_Analog = 0x18010,
   NiFpga_fpga_all_example_IndicatorArrayU32_Counter_Scaler_Digital = 0x18028,
} NiFpga_fpga_all_example_IndicatorArrayU32;

typedef enum
{
   NiFpga_fpga_all_example_IndicatorArrayU32Size_AnalogInputs_Scaler_Analog = 1,
   NiFpga_fpga_all_example_IndicatorArrayU32Size_Counter_Scaler_Analog = 2,
   NiFpga_fpga_all_example_IndicatorArrayU32Size_Counter_Scaler_Digital = 2,
} NiFpga_fpga_all_example_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayBool_Gate_Scaler_Analog = 0x18006,
   NiFpga_fpga_all_example_ControlArrayBool_Gate_Scaler_Digital = 0x18022,
   NiFpga_fpga_all_example_ControlArrayBool_Pulse_ScalerDigital = 0x1803E,
} NiFpga_fpga_all_example_ControlArrayBool;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayBoolSize_Gate_Scaler_Analog = 2,
   NiFpga_fpga_all_example_ControlArrayBoolSize_Gate_Scaler_Digital = 2,
   NiFpga_fpga_all_example_ControlArrayBoolSize_Pulse_ScalerDigital = 1,
} NiFpga_fpga_all_example_ControlArrayBoolSize;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayU32_PresetValues_Scaler_Analog = 0x18000,
   NiFpga_fpga_all_example_ControlArrayU32_PresetValues_Scaler_Digital = 0x1802C,
} NiFpga_fpga_all_example_ControlArrayU32;

typedef enum
{
   NiFpga_fpga_all_example_ControlArrayU32Size_PresetValues_Scaler_Analog = 2,
   NiFpga_fpga_all_example_ControlArrayU32Size_PresetValues_Scaler_Digital = 2,
} NiFpga_fpga_all_example_ControlArrayU32Size;

#endif
