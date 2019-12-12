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
# library setup and exit functions
#
###############################################################################*/

#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"
#include "utils.h"
#include "rt_var_handler.h"
#include "SCALER.h"
#include "WAVEFORM.h"
#include "ANALOG.h"




int crioSetup(struct crio_context *ctx, char *cfgfile) {
    string ip = "";
    string path = "";
    string url = "";
    string bitfile = "";
    string fileName = "";
    string signature = "";
    ctx->use_shared_memory = false;
    string shared_memory_path = "";
    uint32_t shared_memory_size = 0;
    cfg_parser * parser;



    /* Read cfg file */
    if (cfgfile != NULL) {
        TRY_THROW(parser = new cfg_parser(cfgfile));
    } else {
        TRY_THROW(parser = new cfg_parser(CFG_FILE));
    }


    /* Get settings from configuration file */
    TRY_THROW(parser->get_settings(ip, path, fileName, signature, ctx->use_shared_memory, shared_memory_path, shared_memory_size));

    url = "rio://" + ip + "/RIO0";
    bitfile = path + "/" + fileName;

    /* Setting up CRIO */
    auto Res = NiFpga_Initialize();
    try{
        throwCRIOError(Res);
    }  catch(CrioLibException &e) {
        throw (CrioLibException(e.errorcode, "[%s] Failed to initialize FPGA with error:\n%s", LIB_CRIO_LINUX , e.what()));
    }

    NiFpga_Session NiSession;
    Res = NiFpga_Open(bitfile.c_str(), signature.c_str(), url.c_str(), 0, &NiSession);
    if (NiFpga_IsError(Res)) {
        NiFpga_Finalize();
        try{
            throwCRIOError(Res);
        }  catch(CrioLibException &e) {
            throw (CrioLibException(e.errorcode, "[%s] Failed to Open NI session with error:\n%s", LIB_CRIO_LINUX , e.what()));
        }
    }

    ctx->ai_count = 0;
    ctx->bi_count = 0;
    ctx->ao_count = 0;
    ctx->bo_count = 0;
    ctx->mbbo_count = 0;
    ctx->mbbi_count = 0;
    ctx->fxp_count = 0;
    ctx->scaler_count = 0;

    /* Fill in binary maps from the configuration file */
    ctx->bi_map = (void *) new bim_type;
    ctx->bi_addresses = (void *) new bm_address_type;
    ctx->bo_addresses = (void *) new bm_address_type;
    ctx->ao_addresses = (void *) new bm_address_type;
    ctx->ai_addresses = (void *) new bm_address_type;
    ctx->mbbi_addresses = (void *) new bm_address_type;
    ctx->mbbo_addresses = (void *) new bm_address_type;
    ctx->rt_addresses = (void *) new bm_address_type;
    ctx->scaler_name_index_map   = (void *) new bm_address_type;
    ctx->scalers = (void *) new struct scaler_ctx[MAX_SCALER_SUPPORTED_COUNT];
    ctx->waveform_name_index_map   = (void *) new bm_address_type;
    ctx->waveforms = (void *) new struct waveform_ctx[MAX_WAVEFORM_SUPPORTED_COUNT];
    ctx->fxps = (void *) new struct fxp_ctx[MAX_FXP_SUPPORTED_COUNT];
    TRY_THROW(parser->get_bi_maps(ctx->use_shared_memory, ctx->bi_count, (bim_type*) ctx->bi_map, (bm_address_type *)ctx->bi_addresses, (bm_address_type *)ctx->rt_addresses));
    TRY_THROW(parser->get_address_maps(ctx->use_shared_memory, ctx->bo_count, ctx->fxp_count, NULL, (bm_address_type *)ctx->bo_addresses, (bm_address_type *)ctx->rt_addresses, BO_ALIAS));
    TRY_THROW(parser->get_address_maps(ctx->use_shared_memory, ctx->ao_count, ctx->fxp_count, (struct fxp_ctx *) ctx->fxps, (bm_address_type *)ctx->ao_addresses, (bm_address_type *)ctx->rt_addresses, AO_ALIAS));
    TRY_THROW(parser->get_address_maps(ctx->use_shared_memory, ctx->mbbo_count, ctx->fxp_count, (struct fxp_ctx *) ctx->fxps, (bm_address_type *)ctx->mbbo_addresses, (bm_address_type *)ctx->rt_addresses, MBBO_ALIAS));
    TRY_THROW(parser->get_address_maps(ctx->use_shared_memory, ctx->mbbi_count, ctx->fxp_count, (struct fxp_ctx *) ctx->fxps, (bm_address_type *)ctx->mbbi_addresses, (bm_address_type *)ctx->rt_addresses, MBBI_ALIAS));
    TRY_THROW(parser->get_address_maps(ctx->use_shared_memory, ctx->ai_count, ctx->fxp_count, (struct fxp_ctx *) ctx->fxps, (bm_address_type *)ctx->ai_addresses, (bm_address_type *)ctx->rt_addresses, AI_ALIAS));
    TRY_THROW(parser->get_scaler_data((bm_address_type*) ctx->scaler_name_index_map, (struct scaler_ctx *)ctx->scalers));
    TRY_THROW(parser->get_waveform_data(ctx->use_shared_memory, ctx->waveform_fpga_count, (bm_address_type*) ctx->waveform_name_index_map, (bm_address_type *)ctx->rt_addresses, (struct waveform_ctx *)ctx->waveforms));

    /* Calculate offsets if shared memory is enabled */
    if (ctx->use_shared_memory == true) {
        int rt_var_size = ((bm_address_type *)ctx->rt_addresses)->size();
        ctx->rt_variable_offsets = new uint32_t[rt_var_size];
        TRY_THROW(populate_rt_offset_arr(ctx->waveform_name_index_map,
                                         ctx->rt_variable_offsets,
                                         rt_var_size,
                                         ctx->rt_addresses,
                                         (struct waveform_ctx *)ctx->waveforms));
        TRY_THROW(open_shared_memory(shared_memory_path, &ctx->shared_memory, shared_memory_size));

    }

    /* Initialize context */
    ctx->session_open = true;
    ctx->session = (CrioSession)NiSession;
    ctx->bi_cache_valid = false;
    ctx->bi_cache_timeout = 1000;
    ctx->debugCRIO = false;
    Res = pthread_mutex_init(&ctx->bi_mutex,NULL);
    if (Res != 0) throw (CrioLibException(E_RESOURCE_ALLOC, "[%s] Cannot create mutex.", LIB_CRIO_LINUX));

    delete parser;
    return 0;
}



void crioCleanup(struct crio_context *ctx) {
    if (ctx->session_open == true)
    {
        NiFpga_Close(ctx->session, NiFpga_CloseAttribute_NoResetIfLastSession);
        NiFpga_Finalize();
        ctx->session_open = false;
        delete((bm_address_type *)ctx->ai_addresses);
        delete((bm_address_type *)ctx->rt_addresses);
        delete((bm_address_type *)ctx->ao_addresses);
        delete((bm_address_type *)ctx->mbbi_addresses);
        delete((bm_address_type *)ctx->mbbo_addresses);
        delete((bm_address_type *)ctx->bo_addresses);
        delete((bm_address_type *)ctx->bi_addresses);
        delete((bm_address_type *)ctx->scaler_name_index_map);
        delete []((struct scaler_ctx*)ctx->scalers);
        delete((bm_address_type *)ctx->waveform_name_index_map);
        delete []((struct fxp_ctx *)ctx->fxps);
        delete []((struct waveform_ctx *)ctx->waveforms);
        if (ctx->use_shared_memory)
            delete[] ctx->rt_variable_offsets;
        delete((bim_type * )ctx->bi_map);
        pthread_mutex_destroy(&ctx->bi_mutex);
    }
}
