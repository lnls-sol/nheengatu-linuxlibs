#ifndef CRIOLINUX_H
#define CRIOLINUX_H

#include <stdint.h>

#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif

#ifdef __cplusplus
extern "C" {
#endif


/* Type definitions */
typedef	unsigned int CrioSession;

/* Structures */
struct crio_context {
    CrioSession      session;
    bool             session_open;
    clock_t          bi_sample_time;
    uint64_t         bi_cache;
    bool             bi_cache_valid;
    pthread_mutex_t  bi_mutex;
    void           * bi_map;
    void           * bi_addresses;
    void           * bo_addresses;
    void           * ao_addresses;
    void           * ai_addresses;
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
 * - NOK = -1 (Resources could not be allocated)
 * - NOK = -2 (NiFpga_Open failed)
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
 * - NOK = -1 (no item found)
 * - NOK = -2 (Session not open)
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
 * - NOK = -1 (out of range)
 * - NOK = -2 (Session not open)
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
 * - NOK = -2 (Session not open)
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
 * - NOK = -1 (out of range)
 * - NOK = -2 (Session not open)
 */
int crioGetBIArrayItemName(struct crio_context *ctx, unsigned index, const char **name);


/* Function Name  : crioGetBIArrayItemNumber
 * Description    : Gets the BIArray item index using the name
 * Parameters
 * - crio_context : context for the open CRIO session
 * - name         : Name of item
 * - index        : (Return value) Index of item name
 * Return value   :
 * - OK  = 0
 * - NOK = -1 (Item not found)
 * - NOK = -2 (Session not open)
 */
int crioGetBIArrayItemNumber(struct crio_context *ctx, const char *name, unsigned *index);



/* ------------------------------------- BO functions ------------------------------------- */


/* Function Name  : crioGetBOArraySize
 * Description    : Gets the number of BOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of registered BOs available in the ini file
 * Return value   :
 * - OK  = 0
 * - NOK = -2 (Session not open)
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
 * - NOK = -1 (query returned null)
 * - NOK = -2 (Session not open)
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
 * - NOK = -1 (query returned null)
 * - NOK = -2 (Session not open)
 */
int crioSetAOItem(struct crio_context *ctx, const char *name, float value);


/* Function Name  : crioGetAOArraySize
 * Description    : Gets the number of AOs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AOs
 * Return value   :
 * - OK  = 0
 * - NOK = -2 (Session not open)
 */
int crioGetAOArraySize(struct crio_context *ctx, unsigned *size);



/* Function Name  : crioGetAIArraySize
 * Description    : Gets the number of AIs registered by the cfg file
 * Parameters
 * - crio_context : context for the open CRIO session
 * - size         : (Return value) number of the registered AIs
 * Return value   :
 * - OK  = 0
 * - NOK = -2 (Session not open)
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
 * - NOK = -1 (query returned null)
 * - NOK = -2 (Session not open)
 */
int crioGetAIItem(struct crio_context *ctx, const char *name, float &value);

#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
