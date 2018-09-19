#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>

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
    *size = ((bm_address_type *)ctx->ai_addresses)->size();
    return 0;
}


int crioGetAIItem(struct crio_context *ctx, const char *name, float &value) {
    if (!ctx->session_open)
        return -2;
    try {
        return crioGetAI(ctx, ((bm_address_type *)ctx->ai_addresses)->left.at(name), value);
    }
    catch (out_of_range)
    {
        return -1;
    }
}

