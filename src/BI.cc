#include "Common.h"
#include "utils.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"
#include "CrioLinux.h"
#include <sys/types.h>
#include <sys/syscall.h>
#include "cfg_parser.h"

#define CACHE_TIMEOUT_US 1000

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< unsigned, std::string > bim_type;
typedef bimap< std::string, unsigned > bm_address_type;

/* prototypes */
static __inline__ bool getBIVectorItem(struct crio_context *ctx, uint32_t index, uint64_t address);
static __inline__ int crioReadBIArray(struct crio_context *ctx, uint64_t *output, uint64_t address);


/* --------------- HELPERS --------------- */


static __inline__ int crioReadBIArray(struct crio_context *ctx, uint64_t *output, uint64_t address) {

    pthread_mutex_lock(&ctx->bi_mutex);

    struct timespec  current, delta;
    clock_gettime(CLOCK_REALTIME, &current);
    timespec_diff(&ctx->bi_sample_time, &current, &delta);
    double delta_us = delta.tv_sec * 1000000.0 + delta.tv_nsec / 1000.0;
    if ((delta_us < ctx->bi_cache_timeout) && ctx->bi_cache_valid == true)
    {
        *output = ctx->bi_cache;
    } 
    else
    {
        auto Res = NiFpga_ReadU64(NiFpga_Session(ctx->session), address, output);
        if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
        ctx->bi_cache = *output;
        memcpy(&ctx->bi_sample_time, &current, sizeof (struct timespec));
        ctx->bi_cache_valid = true;
    }
    pthread_mutex_unlock(&ctx->bi_mutex);
    return 0;
}

static __inline__ bool getBIVectorItem(struct crio_context *ctx, uint32_t index, uint64_t address){
    uint64_t output;
    try{
        crioReadBIArray(ctx, &output, address);
    } catch(CrioLibException &e) {
        throw (e);
    }

    return (bool) (output & (0x1UL << index));
}

static __inline__ bool getBIScalar(struct crio_context *ctx, uint64_t address){
    bool ret;
    auto Res = NiFpga_ReadBool(NiFpga_Session(ctx->session), address, (NiFpga_Bool*)&ret);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return ret;
}

/* ---------------- API FUNCTIONS ---------------- */

void setBICacheTimeout(struct crio_context *ctx, double timeout)
{
    ctx->bi_cache_timeout = timeout;
}

int crioGetBIArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->bi_count;
    return 0;
}

int crioGetBIArrayItemByName(struct crio_context *ctx, bool *item, const char *name) {
    double value;
    unsigned address ;
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));

    uint64_t index;
    try {
        if (is_rt_var(name) == true)
        {
            uint32_t address =  ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)];
            get_rt_val(ctx->shared_memory, address, value, name);
            *item = static_cast<bool>(value);
            if (ctx->debugCRIO)
            {
                printf ("RT BI name=%s, Value=%d, Offset=0x%05x\n" , name, *item, address);
                fprintf (ctx->log, "RT BI name=%s, Value=%d, Offset=0x%05x\n" , name, *item, address);
            }
        }
        else
        {


            try{

                index = ((bim_type *)ctx->bi_map)->right.at(name);
                address = ((bm_address_type *)ctx->bi_addresses)->left.at(BI_VECTOR);
                *item = getBIVectorItem(ctx, index, address);

            } catch (out_of_range) {
                try{
                    address = ((bm_address_type *)ctx->bi_addresses)->left.at(name);
                    *item = getBIScalar(ctx, address);
                } catch (out_of_range) {
                    throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
                } catch(CrioLibException &e) {
                    throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
                }
            }
            if (ctx->debugCRIO)
            {
                printf ("FPGA BI name=%s, Value=%d, Address=0x%05x, Index=%lu\n" , name, *item, address, index);
                fprintf (ctx->log, "FPGA BI name=%s, Value=%d, Address=0x%05x, Index=%lu\n" , name, *item, address, index);
            }
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

int crioGetBIArrayItemName(struct crio_context *ctx, unsigned index, const char **name) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        *name = ((bim_type *)ctx->bi_map)->left.at(index).c_str();
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , *name ));
    }
    return 0;
}
