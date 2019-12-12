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
# scaler functions
#
###############################################################################*/

#include "Common.h"
#include "utils.h"
#include "SCALER.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "CrioLinux.h"
#include <sys/types.h>
#include <sys/syscall.h>

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< unsigned, std::string > bim_type;
typedef bimap< std::string, unsigned > bm_address_type;


/* ------------ Helpers -------------- */
int static __inline__ getDone(struct crio_context *ctx, uint32_t index, bool *value){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_ReadBool(NiFpga_Session(ctx->session), scaler[index].done_addr, (NiFpga_Bool *) value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ enable(struct crio_context *ctx, uint32_t index){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_WriteBool(NiFpga_Session(ctx->session), scaler[index].enable_addr, true);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ disable(struct crio_context *ctx, uint32_t index){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_WriteBool(NiFpga_Session(ctx->session), scaler[index].enable_addr, false);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ getCounterNum(struct crio_context *ctx, uint32_t index, uint16_t *counters){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    *counters = scaler[index].num_of_counters;
    return 0;
}

int static __inline__ setGates(struct crio_context *ctx, uint32_t index, bool *gates){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_WriteArrayBool(NiFpga_Session(ctx->session), scaler[index].gate_array_addr, (NiFpga_Bool *) gates, scaler[index].num_of_counters);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ setOneShot(struct crio_context *ctx, uint32_t index, bool value){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_WriteBool(NiFpga_Session(ctx->session), scaler[index].oneshot_addr, value);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ getCounters(struct crio_context *ctx, uint32_t index, uint32_t *counters){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_ReadArrayU32(NiFpga_Session(ctx->session), scaler[index].counter_array_addr, counters, scaler[index].num_of_counters);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}

int static __inline__ setPRs(struct crio_context *ctx, uint32_t index, uint32_t *prs){
    struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
    auto Res = NiFpga_WriteArrayU32(NiFpga_Session(ctx->session), scaler[index].pr_array_addr, prs, scaler[index].num_of_counters);
    if (NiFpga_IsError(Res)) throw (CrioLibException(E_VAR_ACCESS, "Cannot access address"));
    return 0;
}


/* ---------------- API FUNCTIONS ---------------- */

int crioSetScalerReset(struct crio_context *ctx, const char * name){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        for (int gate_index = 0; gate_index < MAX_SCALER_CHANNELS; gate_index++)
        {
            crioSetScalerGates(ctx, name, gate_index, false);
            crioSetScalerPresets(ctx, name, gate_index, 0);
        }
        setGates(ctx, scaler_index, ((struct scaler_ctx*)ctx->scalers)[scaler_index].scaler_gate_cache);
        setPRs(ctx, scaler_index, ((struct scaler_ctx*)ctx->scalers)[scaler_index].scaler_preset_cache);
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
           throw (CrioLibException(e.errorcode, "[%s] Property [%s]:[%s] %s.", LIB_CRIO_LINUX , "Scalers", name, e.what()));
    }
    return 0;
}

int crioGetNumOfCounters(struct crio_context *ctx, const char * name, uint16_t *counters){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        getCounterNum(ctx, scaler_index, counters);
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
           throw (CrioLibException(e.errorcode, "[%s] Property [%s]:[%s] %s.", LIB_CRIO_LINUX , "Scalers", name, e.what()));
    }
    return 0;
}

int crioGetScalerCounters(struct crio_context *ctx, const char * name, uint32_t *counters){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        getCounters(ctx, scaler_index, counters);
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
           throw (CrioLibException(e.errorcode, "[%s] Property [%s]:[%s] %s.", LIB_CRIO_LINUX , "Scalers", name, e.what()));
    }
    return 0;
}


int crioSetScalerPresetsGates(struct crio_context *ctx, const char * name, uint32_t preset_index, uint32_t prs){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        crioSetScalerPresets(ctx, name, preset_index, prs);
        crioSetScalerGates(ctx,name, preset_index, true);
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    }
    return 0;
}

int crioSetScalerPresets(struct crio_context *ctx, const char * name, uint32_t preset_index, uint32_t prs){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
        scaler[scaler_index].scaler_preset_cache[preset_index] = prs;
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    }
    return 0;
}

int crioSetScalerGates(struct crio_context *ctx, const char * name, uint32_t gate_index, bool gate){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        struct scaler_ctx* scaler = ((struct scaler_ctx*)(ctx->scalers));
        scaler[scaler_index].scaler_gate_cache[gate_index] = gate;
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    }
    return 0;
}

int crioGetScalerDone(struct crio_context *ctx, const char * name, bool *done){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        getDone(ctx, scaler_index, done);
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
           throw (CrioLibException(e.errorcode, "[%s] Property [%s]:[%s] %s.", LIB_CRIO_LINUX , "Scalers", name, e.what()));
    }
    return 0;
}

int crioSetScalerArm(struct crio_context *ctx, const char * name, bool arm, bool oneshot){
    if (!ctx->session_open)
        throw (CrioLibException(E_SESSION_CLOSED , "[%s] Operation performed on closed session.", LIB_CRIO_LINUX ));
    try {
        uint32_t scaler_index = ((bm_address_type *)ctx->scaler_name_index_map)->left.at(name);
        if (arm == true)
        {
            setGates(ctx, scaler_index, ((struct scaler_ctx*)ctx->scalers)[scaler_index].scaler_gate_cache);
            setPRs(ctx, scaler_index, ((struct scaler_ctx*)ctx->scalers)[scaler_index].scaler_preset_cache);
            setOneShot(ctx, scaler_index, oneshot);
            disable(ctx, scaler_index);
            enable(ctx, scaler_index);
        } else {
            disable(ctx, scaler_index);
        }
    } catch (out_of_range) {
        throw (CrioLibException(E_OUT_OF_RANGE , "[%s] Property Scalers: Query returned null for name %s.", LIB_CRIO_LINUX , name ));
    } catch(CrioLibException &e) {
           throw (CrioLibException(e.errorcode, "[%s] Property [%s]:[%s] %s.", LIB_CRIO_LINUX , "Scalers", name, e.what()));
    }
    return 0;
}
