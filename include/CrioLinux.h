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

#ifdef __cplusplus
extern "C" {
#endif


#define TRY_THROW(x)   try {x;}   \
catch(const CrioLibException &e)  \
{                                 \
throw e;                          \
}                                 \

#define TRY_SILENT(x)  try {x;}   \
catch(const CrioLibException &e)  \
{                                 \
std::cout << e.what() << endl;    \
}                                 \

#define TRY_SILENT_CONT(x)  try {x;}   \
catch(const CrioLibException &e)       \
{                                      \
std::cout << e.what() << endl;         \
continue;                              \
}                                      \
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
    CrioSession      session;
    bool             session_open;
    struct timespec  bi_sample_time;
    uint64_t         bi_cache;
    bool             bi_cache_valid;
    double           bi_cache_timeout;
    pthread_mutex_t  bi_mutex;
    void           * bi_map;
    void           * bi_addresses;
    void           * bo_addresses;
    void           * ao_addresses;
    void           * ai_addresses;
    void           * rt_addresses;
    uint8_t        * rt_variable_offsets;
    uint8_t        * shared_memory;
    uint32_t         ai_count;
    uint32_t         ao_count;
    uint32_t         bi_count;
    uint32_t         bo_count;
};


/* Available Functions */

/* ------------------------------------- Setting up and cleaning ------------------------------------- */



/* Function Name  : crioSetup
 * Description    : Sets up the target crio FPGA and the context.
 * Parameters
 * - crio_context : context to be initialized by the function
 * - cfgfile      : path of the ini configuration file
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

#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
