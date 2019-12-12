/*###############################################################################
#
# This software is distributed under the following ISC license:
#
# Copyright © 2017 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>
#   Dawood Alnajjar <dawood.alnajjar@lnls.br>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Description:
# Waveform functions
#
###############################################################################*/

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
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U08 waveform address at [%x]",
                                                             address));
            break;
        case I08:
            Res = NiFpga_ReadArrayI8(NiFpga_Session(ctx->session), address, (int8_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I08 waveform address at [%x]",
                                                             address));
            break;
        case U16:
            Res = NiFpga_ReadArrayU16(NiFpga_Session(ctx->session), address, (uint16_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U16 waveform address at [%x]",
                                                             address));
            break;
        case I16:
            Res = NiFpga_ReadArrayI16(NiFpga_Session(ctx->session), address, (int16_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I16 waveform address at [%x]",
                                                             address));
            break;
        case U32:
            Res = NiFpga_ReadArrayU32(NiFpga_Session(ctx->session), address, (uint32_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U32 waveform address at [%x]",
                                                             address));
            break;
        case I32:
            Res = NiFpga_ReadArrayI32(NiFpga_Session(ctx->session), address, (int32_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access I32 waveform address at [%x]",
                                                             address));
            break;
        case U64:
            Res = NiFpga_ReadArrayU64(NiFpga_Session(ctx->session), address, (uint64_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U64 waveform address at [%x]",
                                                             address));

            /* Convert to double since epics does not support U64 */
            for (uint i = 0; i < size; i++)
                ((double *)array)[i] = (double)(((uint64_t *)array)[i]);

            break;

        case I64:
            Res = NiFpga_ReadArrayI64(NiFpga_Session(ctx->session), address, (int64_t *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access U64 waveform address at [%x]",
                                                             address));
            /* Convert to double since epics does not support I64 */
            for (uint i = 0; i < size; i++)
                ((double *)array)[i] = (double)(((int64_t *)array)[i]);
            break;
        case SGL:
            Res = NiFpga_ReadArraySgl(NiFpga_Session(ctx->session), address, (float *)array, size);
            if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access SGL waveform address at [%x]",
                                                             address));
            break;
        default :
            throw (CrioLibException(E_NOT_FOUND, "Waveform array not known [%d]", type));
            break;
    }
    return size;
}


int populate_rt_offset_arr(void* waveform_name_index_map, uint32_t * rt_variable_offsets, int rt_var_size, void * rt_addresses, struct waveform_ctx *waveforms)
{
    const char *name;
    uint32_t offset;
    uint32_t waveform_index;

    /* Set first offset to 0 */
    rt_variable_offsets[0] = 0;

    /* Iterate on all items of map from 0 to size-1 and calculate offset of each */

    for (int index = 0; index < rt_var_size-1; index ++)
    {
        try {
            name = ((bm_address_type *)rt_addresses)->right.at(index).c_str();
        }
        catch (out_of_range) {
            throw (CrioLibException(E_OUT_OF_RANGE, "[%s] Cannot find RT item of index <%d>.", LIB_CRIO_LINUX, index));
        }
        offset = rt_variable_offsets[index];

        if (is_waveform((bm_address_type*)waveform_name_index_map, name))
        {
            waveform_index = ((bm_address_type *)waveform_name_index_map)->left.at(name);
            rt_variable_offsets[index+1] = offset + waveforms[waveform_index].waveform_size_bytes;
        }
        else
            rt_variable_offsets[index+1] = offset + decode_enum_size(get_rt_var_size(name));
    }
    return 0;
}

bool is_waveform(void * waveform_name_index_map, const char *name)
{
    try {
        ((bm_address_type*) waveform_name_index_map)->left.at(name);
    } catch (out_of_range) {
        return false;
    }
    return true;
}



/* ------------ API FUNCTIONS -------------- */


int crioGetWaveformItem(struct crio_context *ctx, const char *name, void *array, uint32_t * size, uint64_t max_size) {
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try
    {
        uint32_t waveform_index = ((bm_address_type *)ctx->waveform_name_index_map)->left.at(name);

        if (is_rt_var(name) == true)
        {
            *size = ((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_size_elements;
            if (((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_size_bytes > max_size)
                throw (CrioLibException(E_INI , "EPICS array has smaller size. INI total size <%lu>, EPICS total size <%lu>",
                                        ((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_size_bytes, max_size));
            get_rt_arr(ctx->shared_memory,
                       ctx->rt_variable_offsets[((bm_address_type *)ctx->rt_addresses)->left.at(name)],
                       array,
                       ((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_size_bytes,
                       ((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_type ,
                       ((struct waveform_ctx*)ctx->waveforms)[waveform_index].waveform_size_elements);


        }
        else
        {

            *size = crioGetWaveform(ctx,       (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_addr,
                                                array,
                                               (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_size_elements,
                                               (((struct waveform_ctx*)ctx->waveforms)[waveform_index]).waveform_type);

        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property [%s]: Query returned null.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Property [%s]: %s.", LIB_CRIO_LINUX , name, e.what()));
    }
    return 0;
}

