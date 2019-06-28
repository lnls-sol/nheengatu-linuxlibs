#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"
#include "CrioLinux.h"
#include "utils.h"
#include "ANALOG.h"

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< std::string, unsigned > bm_address_type;

/* ------------ Helpers -------------- */

static __inline__ int crioGetAISingle(struct crio_context *ctx, uint32_t address, float &value);
static __inline__ int crioGetAIFixedPoint(struct crio_context *ctx, double &value, struct fxp_ctx *fxp_data);

static __inline__ int crioGetAISingle(struct crio_context *ctx, uint32_t address, float &value) {
    auto Res = NiFpga_ReadSgl(NiFpga_Session(ctx->session), address, &value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

static __inline__ int crioGetAIFixedPoint(struct crio_context *ctx, double &value, struct fxp_ctx *fxp_data) {
    uint64_t value_u64 = 0 ;
    auto Res = NiFpga_ReadU64(NiFpga_Session(ctx->session), fxp_data->address, &value_u64);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    value = fxp_to_dbl(value_u64, fxp_data);
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
    struct fxp_ctx * local_fxp_data;
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try
    {
        if (is_rt_var(name) == true)
        {
            unsigned address = ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)];
            get_rt_val(ctx->shared_memory, address, value, name);
            if (ctx->debugCRIO)
            {
                printf ("RT AI name=%s, Value=%f, Index=%d\n" , name, value, address);
                fprintf (ctx->log, "RT AI name=%s, Value=%f, Index=%d\n" , name, value, address);
            }
        }
        else if (is_fixed_point(name) == true)
        {
            local_fxp_data = &(((struct fxp_ctx * )ctx->fxps)[((bm_address_type *)ctx->ai_addresses)->left.at(name)]);
            crioGetAIFixedPoint(ctx, value, local_fxp_data);
            if (ctx->debugCRIO)
            {
                printf ("FPGA FXP AI name=%s, Value=%f, Address=0x%08x, Integer word Length=%u, Word Length=%u, Sign=%d\n" , name, value, local_fxp_data->address, local_fxp_data->int_word_length, local_fxp_data->word_length, local_fxp_data->sign);
                fprintf (ctx->log, "FPGA FXP AI name=%s, Value=%f, Address=0x%08x, Integer word Length=%u, Word Length=%u, Sign=%d\n" , name, value, local_fxp_data->address, local_fxp_data->int_word_length, local_fxp_data->word_length, local_fxp_data->sign);
            }
        }
        else
        {
            unsigned address = ((bm_address_type *)ctx->ai_addresses)->left.at(name);
            crioGetAISingle(ctx, address, flt);
            value = (double)(flt);
            if (ctx->debugCRIO)
            {
                printf ("FPGA SGL AI name=%s, Value=%f, Address=0x%08x\n" , name, value, address);
                fprintf (ctx->log, "FPGA SGL AI name=%s, Value=%f, Address=0x%08x\n" , name, value, address);
            }
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

