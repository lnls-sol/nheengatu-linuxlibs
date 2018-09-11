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
static __inline__ int crioSetBO(struct crio_context *ctx, uint32_t address, bool value);


static __inline__ int crioSetBO(struct crio_context *ctx, uint32_t address, bool value) {
    auto Res = NiFpga_WriteBool(NiFpga_Session(ctx->session), address, value);
    if (NiFpga_IsError(Res)) return -1;
    return 0;
}

/* ------------ API FUNCTIONS -------------- */
int crioGetBOArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        return -2;
    *size = ((bm_address_type *)ctx->bo_addresses)->size();
    return 0;
}


int crioSetBOItem(struct crio_context *ctx, const char *name, bool value) {
    if (!ctx->session_open)
        return -2;
    try {
        return crioSetBO(ctx, ((bm_address_type *)ctx->bo_addresses)->left.at(name), value);
    }
    catch (out_of_range)
    {
        cout << "Queried returned NULL for: " << name << endl;
        return -1;
    }
}
