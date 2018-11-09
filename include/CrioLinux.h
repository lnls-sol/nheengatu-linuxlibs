#ifndef CRIOLINUX_H
#define CRIOLINUX_H

#include <stdint.h>
#include <exception>
#include<string.h>
#include <stdarg.h>
#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif

#define TRED "\x1B[31m"
#define TNORM "\x1B[0m"
#define TGREEN "\x1B[32m"
#define TCYAN "\x1B[36m"

#define LIB_CRIO_LINUX "LibCrioLinux"

#define MAX_SCALER_CHANNELS 64

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
    void               * rt_addresses;
    uint8_t            * rt_variable_offsets;
    uint8_t            * shared_memory;
    uint32_t             ai_count;
    uint32_t             ao_count;
    uint32_t             bi_count;
    uint32_t             bo_count;
    uint32_t              num_of_scalers;
    void              *  scalers;
    void               * scaler_name_index_map;
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
 * - NOK = E_NO_MEMORY (Resources could not be allocated)
 * - NOK = E_FPGA_INIT (NiFpga_Open failed)
 * - NOK = E_SHARED_MEM (Failure in accessing shared memory)
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
 * - NOK = E_NOT_FOUND (no item found)
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - NOK = E_OUT_OF_RANGE (out of range)
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - OK  = 0
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - NOK = E_OUT_OF_RANGE (out of range)
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - NOK = E_NOT_FOUND (query returned null)
 * - NOK = E_SESSION_CLOSED (Session not open)
 */
int crioSetBOItem(struct crio_context *ctx, const char *name, bool value);



/* Function Name  : crioSetAOItem
 * Description    : Sets the AO that is associated with the name
 * Parameters
 * - crio_context : context for the open CRIO session
 * - name         : name of AO to be set
 * - value        : value to set the AO with
 * Return value   :
 * - OK  = 0  (Success)
 * - NOK = E_NOT_FOUND (query returned null)
 * - NOK = E_SESSION_CLOSED (Session not open)
 */
int crioSetAOItem(struct crio_context *ctx, const char *name, double value);



/* Function Name  : crioGetAOArraySize
 * Description    : Gets the number of AOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AOs
 * Return value   :
 * - OK  = 0
 * - NOK = E_SESSION_CLOSED (Session not open)
 */
int crioGetAOArraySize(struct crio_context *ctx, unsigned *size);



/* Function Name  : crioGetAIArraySize
 * Description    : Gets the number of AIs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AIs
 * Return value   :
 * - OK  = 0
 * - NOK = E_SESSION_CLOSED (Session not open)
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
 * - NOK = E_NOT_FOUND (query returned null)
 * - NOK = E_SESSION_CLOSED (Session not open)
 */
int crioGetAIItem(struct crio_context *ctx, const char *name, double &value);


/* Function Name  : get_rt_var_size
 * Description    : Gets the RT variable size
 * Parameters
 * - name         : name of RT variable to detect its size
 * Return value   :
 * enum type_code : one of the sizes defined in enum type_code
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
void printVersion(void);


int crioSetScalerReset(struct crio_context *ctx, const char *name);
int crioGetScalerCounters(struct crio_context *ctx, const char * name, uint32_t *counters);
int crioSetScalerPresets(struct crio_context *ctx, const char * name, uint32_t preset_index, uint32_t prs);
int crioGetScalerDone(struct crio_context *ctx, const char * name, bool *done);
int crioSetScalerGates(struct crio_context *ctx, const char * name, uint32_t gate_index, bool gate);
int crioSetScalerArm(struct crio_context *ctx, const char * name, bool arm, bool oneshot);

#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
