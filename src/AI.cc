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

static __inline__ int get_rt_val(struct crio_context *ctx, const char *name, double &value)
{
    double dbl;
    float flt;
    uint8_t u08;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t  i08;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    switch (get_rt_var_size(name))
    {
        case DBL: dbl = *(double*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) dbl;
                  break;
        case SGL: flt = *(float*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) flt;
                  break;
        case U64: u64 = *(uint64_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) u64;
                  break;
        case U32: u32 = *(uint32_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) u32;
                  break;
        case U16: u16 = *(uint16_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) u16;
                  break;
        case U08: u08 = *(uint8_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) u08;
                  break;
        case I64: i64 = *(int64_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) i64;
                  break;
        case I32: i32 = *(int32_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) i32;
                  break;
        case I16: i16 = *(int16_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) i16;
                  break;
        case I08: i08 = *(int8_t*)(ctx->shared_memory + ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)]);
                  value = (double) i08;
                  break;
        default: value = 0; break;
    }
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
            get_rt_val(ctx, name, value);
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

