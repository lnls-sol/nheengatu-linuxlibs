/*
 * Generated with the FPGA Interface C API Generator 17.0.0
 * for NI-RIO 17.0.0 or later.
 */

#ifndef __NiFpga_scaler_analog_digital_2channels_example_h__
#define __NiFpga_scaler_analog_digital_2channels_example_h__

#ifndef NiFpga_Version
   #define NiFpga_Version 1700
#endif

#include "NiFpga.h"

/**
 * The filename of the FPGA bitfile.
 *
 * This is a #define to allow for string literal concatenation. For example:
 *
 *    static const char* const Bitfile = "C:\\" NiFpga_scaler_analog_digital_2channels_example_Bitfile;
 */
#define NiFpga_scaler_analog_digital_2channels_example_Bitfile "NiFpga_scaler_analog_digital_2channels_example.lvbitx"

/**
 * The signature of the FPGA bitfile.
 */
static const char* const NiFpga_scaler_analog_digital_2channels_example_Signature = "B3E9E28DB4178B11290D15C0AFD0704C";

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_IndicatorBool_Done_Scaler_Analog = 0x18026,
   NiFpga_scaler_analog_digital_2channels_example_IndicatorBool_Done_Scaler_Digital = 0x18016,
} NiFpga_scaler_analog_digital_2channels_example_IndicatorBool;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_IndicatorU32_Looptime = 0x18034,
} NiFpga_scaler_analog_digital_2channels_example_IndicatorU32;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_ControlBool_Enable_Scaler_Analog = 0x1801E,
   NiFpga_scaler_analog_digital_2channels_example_ControlBool_Enable_Scaler_Digital = 0x1800E,
   NiFpga_scaler_analog_digital_2channels_example_ControlBool_OneShot_Scaler_Analog = 0x1801A,
   NiFpga_scaler_analog_digital_2channels_example_ControlBool_OneShot_Scaler_Digital = 0x1800A,
   NiFpga_scaler_analog_digital_2channels_example_ControlBool_sample = 0x18032,
} NiFpga_scaler_analog_digital_2channels_example_ControlBool;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32_Counter_Scaler_Analog = 0x18028,
   NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32_Counter_Scaler_Digital = 0x18010,
} NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32Size_Counter_Scaler_Analog = 2,
   NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32Size_Counter_Scaler_Digital = 2,
} NiFpga_scaler_analog_digital_2channels_example_IndicatorArrayU32Size;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBool_Gate_Scaler_Analog = 0x18022,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBool_Gate_Scaler_Digital = 0x18006,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBool_Pulse_Scaler = 0x1803E,
} NiFpga_scaler_analog_digital_2channels_example_ControlArrayBool;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBoolSize_Gate_Scaler_Analog = 2,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBoolSize_Gate_Scaler_Digital = 2,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayBoolSize_Pulse_Scaler = 1,
} NiFpga_scaler_analog_digital_2channels_example_ControlArrayBoolSize;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32_AnalogInputs_Scaler_Analog = 0x18038,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32_PresetValues_Scaler_Analog = 0x1802C,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32_PresetValues_Scaler_Digital = 0x18000,
} NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32;

typedef enum
{
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32Size_AnalogInputs_Scaler_Analog = 2,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32Size_PresetValues_Scaler_Analog = 2,
   NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32Size_PresetValues_Scaler_Digital = 2,
} NiFpga_scaler_analog_digital_2channels_example_ControlArrayU32Size;

#endif
