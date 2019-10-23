#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"
#include "ANALOG.h"
#include "CrioLinux.h"
#include "utils.h"
/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< std::string, unsigned > bm_address_type;

/* ------------ Helpers -------------- */

static __inline__ int crioSetAO(struct crio_context *ctx, uint32_t address, float value);
static __inline__ int crioSetAOFixedPoint(struct crio_context *ctx, double value, struct fxp_ctx *fxp_data) ;


static __inline__ int crioSetAOFixedPoint(struct crio_context *ctx, double value, struct fxp_ctx *fxp_data) {
    uint64_t value_u64 = dbl_to_fxp(value, fxp_data);
    auto Res = NiFpga_WriteU64(NiFpga_Session(ctx->session), fxp_data->address, value_u64);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

static __inline__ int crioSetAO(struct crio_context *ctx, uint32_t address, float value) {
    auto Res = NiFpga_WriteSgl(NiFpga_Session(ctx->session), address, value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}


/* ------------ API FUNCTIONS -------------- */
int crioGetAOArraySize(struct crio_context *ctx, unsigned *size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    *size = ctx->ao_count;
    return 0;
}


int crioSetAOItem(struct crio_context *ctx, const char *name, double value) {
    struct fxp_ctx * local_fxp_data;
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        if (is_rt_var(name) == true)
        {
            unsigned address = ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)];
            set_rt_val(ctx->shared_memory, address, value, name);
            if (ctx->debugCRIO)
            {
                printf ("RT AO name=%s, Value=%f, Index=%d\n" , name, value, address);
                fprintf (ctx->log, "RT AO name=%s, Value=%f, Index=%d\n" , name, value, address);
            }
        }
        else
        {
            if (is_fixed_point(name) == true)
            {
                local_fxp_data = &(((struct fxp_ctx * )ctx->fxps)[((bm_address_type *)ctx->ao_addresses)->left.at(name)]);
                crioSetAOFixedPoint(ctx, value, local_fxp_data);
                if (ctx->debugCRIO)
                {
                    printf ("FPGA FXP AO name=%s, Value=%f, Address=0x%05x, Integer word Length=%u, Word Length=%u, Sign=%d\n" , name, value, local_fxp_data->address, local_fxp_data->int_word_length, local_fxp_data->word_length, local_fxp_data->sign);
                    fprintf (ctx->log, "FPGA FXP AO name=%s, Value=%f, Address=0x%05x, Integer word Length=%u, Word Length=%u, Sign=%d\n" , name, value, local_fxp_data->address, local_fxp_data->int_word_length, local_fxp_data->word_length, local_fxp_data->sign);
                }
            }
            else
            {
                unsigned address = ((bm_address_type *)ctx->ao_addresses)->left.at(name);
                crioSetAO(ctx, address, (float)value);
                if (ctx->debugCRIO)
                {
                    printf ("FPGA SGL AO name=%s, Value=%f, Address=0x%05x\n" , name, value, address);
                    fprintf (ctx->log, "FPGA SGL AO name=%s, Value=%f, Address=0x%05x\n" , name, value, address);
                }
            }
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

