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

#define CACHE_TIMEOUT_US 1000

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< unsigned, std::string > bim_type;
typedef bimap< std::string, unsigned > bm_address_type;

/* prototypes */
static __inline__ bool getBI(struct crio_context *ctx, uint32_t index, uint64_t address);
static __inline__ int crioReadBIArray(struct crio_context *ctx, uint64_t *output, uint64_t address);
static __inline__ int ParseNumberStrict(const char *Text, unsigned *Value);

/* --------------- HELPERS --------------- */
static __inline__ int ParseNumberStrict(const char *Text, unsigned *Value) {
    char *End;
    long long Val;

    /* Blank string returns error */
    while (*Text != '\0' && isspace(*Text)) Text++;
    if (*Text == '\0') return -1;

    /* Use signed parsing to avoid implicit modular conversion */
    errno = 0;
    Val = strtoll(Text, &End, 0);
    if (errno != 0) return -1;

    /* Trailing text returns error */
    while (*End != '\0' && isspace(*End)) End++;
    if (*End != '\0') return -1;

    /* Handle unsigned range errors */
    if (Val < 0) return -1;
    if (Val > (long long)UINT_MAX) return -1;

    *Value = Val;

    return 0;
}




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
        if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address."));
        ctx->bi_cache = *output;
        memcpy(&ctx->bi_sample_time, &current, sizeof (struct timespec));
        ctx->bi_cache_valid = true;
    }
    pthread_mutex_unlock(&ctx->bi_mutex);
    return 0;
}

static __inline__ bool getBI(struct crio_context *ctx, uint32_t index, uint64_t address){
    uint64_t output;
    try{
        crioReadBIArray(ctx, &output, address);
    } catch(CrioLibException &e) {
        throw (e);
    }

    return (bool) (output & (0x1UL << index));
}

/* ---------------- API FUNCTIONS ---------------- */

void setBICacheTimeout(struct crio_context *ctx, double timeout)
{
    ctx->bi_cache_timeout = timeout;
}

int crioGetBIArrayItemByIndex(struct crio_context *ctx, bool *item, uint32_t index) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        ((bim_type *)ctx->bi_map)->left.at(index).c_str();
        *item = getBI(ctx, index, ((bm_address_type *)ctx->bi_addresses)->left.at("BI0") );
        return 0;
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property <BI0>: Query returned null for index %d.", LIB_CRIO_LINUX , index ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property <%s>: %s.", LIB_CRIO_LINUX , "BI0", e.what()));
    }
}

int crioGetBIArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->bi_count;
    return 0;
}

int crioGetBIArrayItemByName(struct crio_context *ctx, bool *item, const char *name) {
    double value;
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));

    uint64_t index;
    try {
        if (is_rt_var(name) == true)
        {
            get_rt_val(ctx->shared_memory, ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)], value, name);
            *item = static_cast<bool>(value);
        }
        else
        {

            index = ((bim_type *)ctx->bi_map)->right.at(name);
            *item = getBI(ctx, index, ((bm_address_type *)ctx->bi_addresses)->left.at("BI0"));
        }
        return 0;
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property <%s>: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property <%s>: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
}

int crioGetBIArrayItemName(struct crio_context *ctx, unsigned index, const char **name) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        *name = ((bim_type *)ctx->bi_map)->left.at(index).c_str();
        return 0;
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property <%s>: Query returned null.", LIB_CRIO_LINUX , *name ));
    }
}
