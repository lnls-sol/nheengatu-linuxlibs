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
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address."));
    return 0;
}


/* ------------ API FUNCTIONS -------------- */
int crioGetAIArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->ai_count;
    return 0;
}


int crioGetAIItem(struct crio_context *ctx, const char *name, double &value) {
    float flt;
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try
    {
        if (is_rt_var(name) == true)
            get_rt_val(ctx->shared_memory, ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)], value, name);
        else
        {
            crioGetAI(ctx, ((bm_address_type *)ctx->ai_addresses)->left.at(name), flt);
            value = (double)(flt);
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

