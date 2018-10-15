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

static __inline__ int crioGetAI(struct crio_context *ctx, uint32_t address, float &value);


static __inline__ int crioGetAI(struct crio_context *ctx, uint32_t address, float &value) {
    auto Res = NiFpga_ReadSgl(NiFpga_Session(ctx->session), address, &value);
    if (NiFpga_IsError(Res)) return -1;
    return 0;
}


/* ------------ API FUNCTIONS -------------- */
int crioGetAIArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        return -2;
    *size = ctx->ai_count;
    return 0;
}


int crioGetAIItem(struct crio_context *ctx, const char *name, double &value) {
    int Res = 0;
    float flt;
    if (!ctx->session_open)
        return -2;
    try
    {
        if (is_rt_var(name) == true)
            get_rt_val(ctx->shared_memory, ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)], value, name);
        else
        {
            Res = crioGetAI(ctx, ((bm_address_type *)ctx->ai_addresses)->left.at(name), flt);
            value = (double)(flt);
            return Res;
        }
        return 0;
    }
    catch (out_of_range)
    {
        return -1;
    }
}

