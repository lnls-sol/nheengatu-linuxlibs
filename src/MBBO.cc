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

static __inline__ int crioSetMBBO(struct crio_context *ctx, uint32_t address, uint16_t value);


static __inline__ int crioSetMBBO(struct crio_context *ctx, uint32_t address, uint16_t value) {
    auto Res = NiFpga_WriteU16(NiFpga_Session(ctx->session), address, value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}


/* ------------ API FUNCTIONS -------------- */
int crioGetMBBOArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->mbbo_count;
    return 0;
}


int crioSetMBBOItem(struct crio_context *ctx, const char *name, double value) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        if (is_rt_var(name) == true)
        {
            unsigned address = ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)];
            set_rt_val(ctx->shared_memory, address, value, name);
            if (ctx->debugCRIO)
            {
                printf ("RT MBBO name=%s, Value=%f, Index=%d\n" , name, value, address);
                fprintf (ctx->log, "RT MBBO name=%s, Value=%f, Index=%d\n" , name, value, address);
            }
        }
        else
        {
            unsigned address = ((bm_address_type *)ctx->mbbo_addresses)->left.at(name);
            crioSetMBBO(ctx, address, (uint16_t)value);
            if (ctx->debugCRIO)
            {
                printf ("FPGA MBBO name=%s, Value=%f, Address=0x%05x\n" , name, value, address);
                fprintf (ctx->log, "FPGA MBBO name=%s, Value=%f, Address=0x%05x\n" , name, value, address);
            }
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

