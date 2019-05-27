#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"
#include "CrioLinux.h"
#include "WAVEFORM.h"

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< std::string, unsigned > bm_address_type;

/* ------------ Helpers -------------- */

static __inline__ uint32_t crioGetWaveform(struct crio_context *ctx, uint32_t address, void *array, uint32_t size, enum type_code type);


static __inline__ uint32_t crioGetWaveform(struct crio_context *ctx, uint32_t address, void *array,
                                      uint32_t size, enum type_code type) {
    int Res = 0;

    switch (type)
    {
        case U08:
            Res = NiFpga_ReadArrayU8(NiFpga_Session(ctx->session), address, (uint8_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U08 waveform address at [%x].",
                                                             address));
            break;
        case I08:
            Res = NiFpga_ReadArrayI8(NiFpga_Session(ctx->session), address, (int8_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I08 waveform address at [%x].",
                                                             address));
            break;
        case U16:
            Res = NiFpga_ReadArrayU16(NiFpga_Session(ctx->session), address, (uint16_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U16 waveform address at [%x].",
                                                             address));
            break;
        case I16:
            Res = NiFpga_ReadArrayI16(NiFpga_Session(ctx->session), address, (int16_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I16 waveform address at [%x].",
                                                             address));
            break;
        case U32:
            Res = NiFpga_ReadArrayU32(NiFpga_Session(ctx->session), address, (uint32_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U32 waveform address at [%x].",
                                                             address));
            break;
        case I32:
            Res = NiFpga_ReadArrayI32(NiFpga_Session(ctx->session), address, (int32_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I32 waveform address at [%x].",
                                                             address));
            break;
        case U64:
            Res = NiFpga_ReadArrayU64(NiFpga_Session(ctx->session), address, (uint64_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U64 waveform address at [%x].",
                                                             address));

            /* Convert to double since epics does not support U64 */
            for (uint i = 0; i < size; i++)
                ((double *)array)[i] = (double)(((uint64_t *)array)[i]);

            break;

        case I64:
            Res = NiFpga_ReadArrayI64(NiFpga_Session(ctx->session), address, (int64_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U64 waveform address at [%x].",
                                                             address));
            /* Convert to double since epics does not support I64 */
            for (uint i = 0; i < size; i++)
                ((double *)array)[i] = (double)(((int64_t *)array)[i]);
            break;
        case SGL:
            Res = NiFpga_ReadArraySgl(NiFpga_Session(ctx->session), address, (float *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access SGL waveform address at [%x].",
                                                             address));
            break;
        default :
            throw (CrioLibException(E_NOT_FOUND, "Waveform array not known [%d].", type));
            break;
    }
    return size;
}


/* ------------ API FUNCTIONS -------------- */


int crioGetWaveformItem(struct crio_context *ctx, const char *name, void *array, uint32_t * size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try
    {
        if (is_rt_var(name) == true)
            printf("TODO: Need to Build RT of arrays\n");
            //get_rt_val(ctx->shared_memory, ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)], value, name);
        else
        {
            uint32_t waveform_index = ((bm_address_type *)ctx->waveform_name_index_map)->left.at(name);
            *size = crioGetWaveform(ctx,       (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_addr,
                                                array,
                                               (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_size,
                                               (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_type);

        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

