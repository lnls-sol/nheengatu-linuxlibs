/*###############################################################################
#
# This software is distributed under the following ISC license:
#
# Copyright © 2017 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>
#   Dawood Alnajjar <dawood.alnajjar@lnls.br>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Description:
# Library header file. Will be included by all applications linking
# with this library.
#
###############################################################################*/

#ifndef CRIOLINUX_H
#define CRIOLINUX_H

#include <stdint.h>
#include <exception>
#include<string.h>
#include <stdarg.h>
#include <stdlib.h>

#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif

#define TRED "\x1B[31m"
#define TNORM "\x1B[0m"
#define TGREEN "\x1B[32m"
#define TCYAN "\x1B[36m"

#define LIB_CRIO_LINUX "LibCrioLinux"

//TODO: resolve compilation from C code since we now use exceptions.
#ifdef __cplusplus
extern "C" {
#endif


#define TRY_THROW(x)  do{try {x;} \
catch(const CrioLibException &e)  \
{                                 \
throw e;                          \
}                                 \
} while(0)

#define TRY_SILENT(x) do {try {x;}\
catch(const CrioLibException &e)  \
{                                 \
std::cout << e.what() << endl;    \
}                                 \
} while(0)

#define TRY_SILENT_CONT(x) do {try {x;}\
catch(const CrioLibException &e)       \
{                                      \
std::cout << e.what() << endl;         \
continue;                              \
}                                      \
} while(0)


/* Error codes that will be passed with the Exception */
enum errorcodes{
    E_NO_MEMORY = -1,
    E_FPGA_INIT = -2,
    E_SHARED_MEM = -3,
    E_SESSION_CLOSED = -4,
    E_OUT_OF_RANGE = -5,
    E_NOT_FOUND = -6,
    E_INI = -7,
    E_SAME_ADDRESS = -8,
    E_RESOURCE_ALLOC = -9,
    E_VAR_ACCESS = -10
};

enum type_code{
    DBL,
    SGL,
    U64,
    U32,
    U16,
    U08,
    I64,
    I32,
    I16,
    I08,
    BOL,
    STO,
    STI,
    UNKNOWN
};

/* exception structure that will be captured by the software */
struct CrioLibException : public std::exception {
    char error_text[1000];
    enum errorcodes errorcode;
    CrioLibException(enum errorcodes code, char const* fmt, ...) __attribute__((format(printf,3,4))) {
            va_list ap;
            va_start(ap, fmt);
            vsnprintf(error_text, sizeof error_text, fmt, ap);
            va_end(ap);
        errorcode = code;
    }
   const char * what () const throw () {
      return error_text;
   }
};


/* Type definitions */
typedef	unsigned int CrioSession;


/* Structures */
struct crio_context {
    void               * waveform_name_index_map;
    CrioSession          session;
    bool                 session_open;
    struct timespec      bi_sample_time;
    uint64_t             bi_cache;
    bool                 bi_cache_valid;
    double               bi_cache_timeout;
    pthread_mutex_t      bi_mutex;
    void               * bi_map;
    void               * bi_addresses;
    void               * bo_addresses;
    void               * ao_addresses;
    void               * ai_addresses;
    void               * mbbi_addresses;
    void               * mbbo_addresses;
    void               * rt_addresses;
    uint32_t           * rt_variable_offsets;
    uint8_t            * shared_memory;
    bool                 use_shared_memory;
    uint16_t             ai_count;
    uint16_t             ao_count;
    uint16_t             mbbo_count;
    uint16_t             mbbi_count;
    uint16_t             stringout_count;
    uint16_t             stringin_count;
    uint16_t             bi_count;
    uint16_t             bo_count;
    uint16_t             fxp_count;
    uint16_t             scaler_count;
    uint16_t             waveform_fpga_count;
    void               * scalers;
    void               * scaler_name_index_map;
    void               * waveforms;
    void               * fxps;
    bool                 debugCRIO;
    FILE               * log;
};


/* Available Functions */

/* ------------------------------------- Setting up and cleaning ------------------------------------- */



/* Function Name  : crioSetup
 * Description    : Sets up the target crio FPGA and the context.
 * Parameters
 * - crio_context : context to be initialized by the function
 * - cfgfile      : path of the ini configuration file. If
 *                  NULL is passed, the default location and filename
 *                  are used: /usr/local/epics/config/cfg.ini
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetup(struct crio_context *ctx, char * cfgfile);



/* Function Name  : crioCleanup
 * Description    : Cleans up a session.
 * Parameters
 * - crio_context : context for the open CRIO session
 * Return value   :
 * - OK  = 0
 */
void crioCleanup(struct crio_context* ctx);



/* ------------------------------------- BI functions ------------------------------------- */


/* Function Name  : CrioGetBIArrayItemByName
 * Description    : Gets the BIArray item using the name as a key
 * Parameters
 * - crio_context : context for the open CRIO session
 * - item         : (Return value) Item to be obtained using name as a key
 * - name         : key to obtain value (item)
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetBIArrayItemByName(struct crio_context *ctx, bool *item, const char *name);




/* Function Name  : crioGetBIArrayItemByIndex
 * Description    : Gets the BIArray item using the index as a key.
 *                  This function needs to be rethought when more
 *                  than one BI array is necessary.
 * Parameters
 * - crio_context : context for the open CRIO session
 * - item         : (Return value) Item to be obtained using name as a key
 * - index        : key to obtain value (item)
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetBIArrayItemByIndex(struct crio_context*, bool *item, uint32_t index);



/* Function Name  : crioGetBIArraySize
 * Description    : Gets the BIArray size. This function needs to
 *                  be rethought when more than one BI array is
 *                  necessary.
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) size to be obtained
 * Return value   :
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetBIArraySize(struct crio_context *ctx, uint32_t *size);



/* Function Name  : crioGetBIArrayItemName
 * Description    : Gets the BIArray item name using the index
 * Parameters
 * - crio_context : context for the open CRIO session
 * - index        : Index of item name
 * - name         : (Return value) Name of item
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetBIArrayItemName(struct crio_context *ctx, unsigned index, const char **name);


/* Function Name  : setBICacheTimeout
 * Description    : Set BI cache timeout
 * Parameters
 * - timeout : timeout in Microseconds for BI cache
 */
void setBICacheTimeout(struct crio_context *ctx, double timeout);

/* ------------------------------------- BO functions ------------------------------------- */


/* Function Name  : crioGetBOArraySize
 * Description    : Gets the number of BOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of registered BOs available in the ini file
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetBOArraySize(struct crio_context *ctx, unsigned *size);


/* Function Name  : crioSetBOItem
 * Description    : Sets the BO that is associated with the name
 * Parameters
 * - crio_context : context for the open CRIO session
 * - name         : name of BO to be set
 * - value        : value to set the BO with
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetBOItem(struct crio_context *ctx, const char *name, bool value);


/* ------------------------------------- AO functions ------------------------------------- */

/* Function Name  : crioSetAOItem
 * Description    : Sets the AO that is associated with the name
 * Parameters
 * - crio_context : context for the open CRIO session
 * - name         : name of AO to be set
 * - value        : value to set the AO with
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetAOItem(struct crio_context *ctx, const char *name, double value);



/* Function Name  : crioGetAOArraySize
 * Description    : Gets the number of AOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AOs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetAOArraySize(struct crio_context *ctx, unsigned *size);



/* ------------------------------------- AI functions ------------------------------------- */

/* Function Name  : crioGetAIArraySize
 * Description    : Gets the number of AIs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AIs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetAIArraySize(struct crio_context *ctx, unsigned *size);


/* Function Name  : crioGetAIItem
 * Description    : Sets the AI that is associated with the name
 * Parameters
 * - crio_context : context for the open CRIO session
 * - name         : name of AI to be set
 * - value        : value to set the AI with
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetAIItem(struct crio_context *ctx, const char *name, double &value);


/* ------------------------------------- RT functions ------------------------------------- */

/* Function Name  : get_rt_var_size
 * Description    : Gets the RT variable size
 * Parameters
 * - name         : name of RT variable to detect its size
 * Return value   :
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
enum type_code get_rt_var_size(std::string name);


/* Function Name  : is_rt_var
 * Description    : Checks if the name passed is an RT variable
 * Parameters
 * - name         : name of RT variable to detect its size
 * Return value   :
 * true  = is an RT variable
 * false = is not an RT variable
 */
bool is_rt_var(std::string name);


/* This function prints build information
 * of the CrioLinux library.
 */
void printLibVersion(void);


/* ------------------------------------- Scaler functions ------------------------------------- */

/* Function Name  : crioSetScalerReset
 * Description    : Resets CRIO scaler module, and sets all gate and preset values to zero
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetScalerReset(struct crio_context *ctx, const char *name);

/* Function Name  : crioGetScalerCounters
 * Description    : gets all 64 scaler counter values
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - counters     : counter values will be returned in this array
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetScalerCounters(struct crio_context *ctx, const char * name, uint32_t *counters);

/* Function Name  : crioSetScalerPresets
 * Description    : set a single preset value in preset cache
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - preset_index : Index of the preset to be set
 * - prs          : value of the preset
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetScalerPresets(struct crio_context *ctx, const char * name, uint32_t preset_index, uint32_t prs);


/* Function Name  : crioSetScalerPresetsGates
 * Description    : set a single preset value in preset cache and
 *                  sets associate gate value to 1.
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - preset_index : Index of the preset to be set
 * - prs          : value of the preset
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetScalerPresetsGates(struct crio_context *ctx, const char * name, uint32_t preset_index, uint32_t prs);


/* Function Name  : crioGetScalerDone
 * Description    : checks if hardware is done
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - done         : if done, function will set this value to true
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetScalerDone(struct crio_context *ctx, const char * name, bool *done);

/* Function Name  : crioSetScalerGates
 * Description    : Sets a single gate to <gate>
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - gate_index   : index if the gate to set
 * - gate         : value to set the gate with
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetScalerGates(struct crio_context *ctx, const char * name, uint32_t gate_index, bool gate);

/* Function Name  : crioSetScalerArm
 * Description    : arms the scaler. This functions writes the gate
 *                  and preset caches to hardware.
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - arm          : arm or disarm the scaler hardware
 * - oneshot      : set to true, oneshot will be enabled. Otherwise
 *                  autocount will be used.
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetScalerArm(struct crio_context *ctx, const char * name, bool arm, bool oneshot);


/* Function Name  : crioGetNumOfCounters
 * Description    : Get the number of implemented counters
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of scaler defined in the configuration file
 * - counters     : a pointer to a value that will be returned by the function
 *                  (number of channels)
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetNumOfCounters(struct crio_context *ctx, const char * name, uint16_t *counters);



/* ------------------------------------- Waveform functions ------------------------------------- */


/* Function Name  : crioGetWaveformItem
 * Description    : read waveform
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of the waveform
 * - array        : a pointer to the array to be filled
 * - size         : the number of variables read
 * - max_size     : Epics defined array size (used for safety check).
 *
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */

int crioGetWaveformItem(struct crio_context *ctx, const char *name, void *array, uint32_t *size, uint64_t max_size);


/* ------------------------------------- MBBI functions ------------------------------------- */


/* Function Name  : crioGetMBBIItem
 * Description    : read MBBI
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of the MBBI
 * - value        : readback value of MBBI
 *
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetMBBIItem(struct crio_context *ctx, const char *name, double &value);


/* Function Name  : crioGetMBBIArraySize
 * Description    : Gets the number of MBBIs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered MBBIs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetMBBIArraySize(struct crio_context *ctx, unsigned *size);

/* ------------------------------------- MBBO functions ------------------------------------- */


/* Function Name  : crioGetMBBOItem
 * Description    : read MBBO
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of the MBBO
 * - value        : readback value of MBBO
 *
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetMBBOItem(struct crio_context *ctx, const char *name, double value);


/* Function Name  : crioGetMBBOArraySize
 * Description    : Gets the number of MBBOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered MBBOs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetMBBOArraySize(struct crio_context *ctx, unsigned *size);


/* ------------------------------------- STRINGIN functions ------------------------------------- */
/* Function Name  : crioGetSTRINGINItem
 * Description    : read STRINGIN
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of the STRINGIN
 * - value        : readback value of STRINGIN
 *
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetSTRINGINItem(struct crio_context *ctx, const char *name, char value[40]);


/* Function Name  : crioGetSTRINGINArraySize
 * Description    : Gets the number of STRINGINs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered STRINGINs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetSTRINGINArraySize(struct crio_context *ctx, unsigned *size);

/* ------------------------------------- STRINGOUT functions ------------------------------------- */
/* Function Name  : crioSetSTRINGOUTItem
 * Description    : write STRINGOUT
 * Parameters
 * - crio_context : context for the open CRIO session*
 * - name         : name of the STRINGOUT
 * - value        : STRINGOUT to write
 *
 * Return value   :
 * - OK  = 0  (Success)
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioSetSTRINGOUTItem(struct crio_context *ctx, const char *name, char value[40]);


/* Function Name  : crioGetSTRINGOUTArraySize
 * Description    : Gets the number of STRINGOUTs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered STRINGOUTs
 * Return value   :
 * - OK  = 0
 * otherwise an exception will be thrown with the
 * one of the errorcodes defined in errorcodes.
 */
int crioGetSTRINGOUTArraySize(struct crio_context *ctx, unsigned *size);


/* Function to set debug on */
void debug(crio_context *ctx, int enable);



#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
