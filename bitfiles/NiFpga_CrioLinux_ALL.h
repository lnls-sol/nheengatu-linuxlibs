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
static const char* const NiFpga_CrioLinux_Signature = "90B69ED82168C42A367ABB4971EF69BC";

typedef enum
{
   NiFpga_CrioLinux_IndicatorU64_BI = 0x18064,
} NiFpga_CrioLinux_IndicatorU64;

typedef enum
{
   NiFpga_CrioLinux_IndicatorSgl_Mod4AI0 = 0x18074,
   NiFpga_CrioLinux_IndicatorSgl_Mod4AI1 = 0x18070,
   NiFpga_CrioLinux_IndicatorSgl_Mod4AI2 = 0x1806C,
   NiFpga_CrioLinux_IndicatorSgl_Mod4AI3 = 0x18068,
   NiFpga_CrioLinux_IndicatorSgl_Mod6TC0 = 0x18094,
   NiFpga_CrioLinux_IndicatorSgl_Mod6TC1 = 0x18090,
   NiFpga_CrioLinux_IndicatorSgl_Mod6TC2 = 0x1808C,
   NiFpga_CrioLinux_IndicatorSgl_Mod6TC3 = 0x18088,
   NiFpga_CrioLinux_IndicatorSgl_Mod7AI0 = 0x180A4,
   NiFpga_CrioLinux_IndicatorSgl_Mod7AI1 = 0x180A0,
   NiFpga_CrioLinux_IndicatorSgl_Mod7AI2 = 0x1809C,
   NiFpga_CrioLinux_IndicatorSgl_Mod7AI3 = 0x18098,
   NiFpga_CrioLinux_IndicatorSgl_Mod8AI0 = 0x18000,
   NiFpga_CrioLinux_IndicatorSgl_Mod8AI1 = 0x180A8,
   NiFpga_CrioLinux_IndicatorSgl_Mod8AI2 = 0x180AC,
   NiFpga_CrioLinux_IndicatorSgl_Mod8AI3 = 0x180B0,
} NiFpga_CrioLinux_IndicatorSgl;

typedef enum
{
   NiFpga_CrioLinux_ControlBool_Mod1DIO0 = 0x18056,
   NiFpga_CrioLinux_ControlBool_Mod1DIO1 = 0x1805A,
   NiFpga_CrioLinux_ControlBool_Mod1DIO2 = 0x1805E,
   NiFpga_CrioLinux_ControlBool_Mod1DIO3 = 0x18062,
   NiFpga_CrioLinux_ControlBool_Mod2DIO0 = 0x18046,
   NiFpga_CrioLinux_ControlBool_Mod2DIO1 = 0x1804A,
   NiFpga_CrioLinux_ControlBool_Mod2DIO2 = 0x1804E,
   NiFpga_CrioLinux_ControlBool_Mod2DIO3 = 0x18052,
   NiFpga_CrioLinux_ControlBool_Mod3DIO16 = 0x18042,
   NiFpga_CrioLinux_ControlBool_Mod3DIO17 = 0x1803E,
   NiFpga_CrioLinux_ControlBool_Mod3DIO18 = 0x1803A,
   NiFpga_CrioLinux_ControlBool_Mod3DIO19 = 0x18036,
   NiFpga_CrioLinux_ControlBool_Mod3DIO20 = 0x18032,
   NiFpga_CrioLinux_ControlBool_Mod3DIO21 = 0x1802E,
   NiFpga_CrioLinux_ControlBool_Mod3DIO22 = 0x1802A,
   NiFpga_CrioLinux_ControlBool_Mod3DIO23 = 0x18026,
   NiFpga_CrioLinux_ControlBool_Mod3DIO24 = 0x18022,
   NiFpga_CrioLinux_ControlBool_Mod3DIO25 = 0x1801E,
   NiFpga_CrioLinux_ControlBool_Mod3DIO26 = 0x1801A,
   NiFpga_CrioLinux_ControlBool_Mod3DIO27 = 0x18016,
   NiFpga_CrioLinux_ControlBool_Mod3DIO28 = 0x18012,
   NiFpga_CrioLinux_ControlBool_Mod3DIO29 = 0x1800E,
   NiFpga_CrioLinux_ControlBool_Mod3DIO30 = 0x1800A,
   NiFpga_CrioLinux_ControlBool_Mod3DIO31 = 0x18006,
} NiFpga_CrioLinux_ControlBool;

typedef enum
{
   NiFpga_CrioLinux_ControlSgl_Mod5AO0 = 0x18078,
   NiFpga_CrioLinux_ControlSgl_Mod5AO1 = 0x1807C,
   NiFpga_CrioLinux_ControlSgl_Mod5AO2 = 0x18080,
   NiFpga_CrioLinux_ControlSgl_Mod5AO3 = 0x18084,
} NiFpga_CrioLinux_ControlSgl;

#endif
