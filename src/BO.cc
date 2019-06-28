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
static __inline__ int crioSetBO(struct crio_context *ctx, uint32_t address, bool value);


static __inline__ int crioSetBO(struct crio_context *ctx, uint32_t address, bool value) {
    auto Res = NiFpga_WriteBool(NiFpga_Session(ctx->session), address, value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

/* ------------ API FUNCTIONS -------------- */
int crioGetBOArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->bo_count;
    return 0;
}


int crioSetBOItem(struct crio_context *ctx, const char *name, bool value) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        if (is_rt_var(name) == true)
        {
            uint32_t offset = ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)];
            set_rt_val(ctx->shared_memory, offset, static_cast<double>(value), name);
            if (ctx->debugCRIO)
            {
                printf ("RT BO name=%s, Value=%d, Offset=0x%05x\n" , name, value, offset);
                fprintf (ctx->log, "RT BO name=%s, Value=%d, Offset=0x%05x\n" , name, value, offset);
            }
            return 0;
        }
        else
        {
            uint32_t address = ((bm_address_type *)ctx->bo_addresses)->left.at(name);
            if (ctx->debugCRIO)
            {
                printf ("FPGA BO name=%s, Value=%d, Address=0x%05x\n" , name, value, address);
                fprintf (ctx->log, "FPGA BO name=%s, Value=%d, Address=%05x\n" , name, value, address);
            }
            return crioSetBO(ctx, address, value);
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
}
