#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"

#include "CrioLinux.h"

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< std::string, unsigned > bm_address_type;

/* ------------ Helpers -------------- */

static __inline__ int crioSetAO(struct crio_context *ctx, uint32_t address, float value);


static __inline__ int crioSetAO(struct crio_context *ctx, uint32_t address, float value) {
    auto Res = NiFpga_WriteSgl(NiFpga_Session(ctx->session), address, value);
    if (NiFpga_IsError(Res)) return -1;
    return 0;
}


/* ------------ API FUNCTIONS -------------- */
int crioGetAOArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        return -2;
    *size = ctx->ao_count;
    return 0;
}


int crioSetAOItem(struct crio_context *ctx, const char *name, double value) {
    if (!ctx->session_open)
        return -2;
    try {
        if (is_rt_var(name) == true)
        {
            set_rt_val(ctx->shared_memory, ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)], value, name);
            return 0;
        }
        else
           return crioSetAO(ctx, ((bm_address_type *)ctx->ao_addresses)->left.at(name), (float)value);
    }
    catch (out_of_range)
    {
        return -1;
    }
}

