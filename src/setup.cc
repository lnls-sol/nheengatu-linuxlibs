#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"
#include "utils.h"
#include "rt_var_handler.h"
#include "SCALER.h"

int crioSetup(struct crio_context *ctx, char *cfgfile) {
    string ip = "";
    string path = "";
    string url = "";
    string bitfile = "";
    string fileName = "";
    string signature = "";
    bool use_shared_memory = false;
    string shared_memory_path = "";
    const char *name;
    cfg_parser * parser;


    if (ctx->session_open == false)
    {
        /* Read cfg file */
        if (cfgfile != NULL) {
            TRY_THROW(parser = new cfg_parser(cfgfile));
        } else {
            TRY_THROW(parser = new cfg_parser(CFG_FILE));
        }


        /* Get settings from configuration file */
        TRY_THROW(parser->get_settings(ip, path, fileName, signature, use_shared_memory, shared_memory_path));

        url = "rio://" + ip + "/RIO0";
        bitfile = path + "/" + fileName;

        /* Setting up CRIO */
        auto Res = NiFpga_Initialize();
        if (NiFpga_IsError(Res))
            throw (CrioLibException(E_FPGA_INIT, "[%s] Failed to initialize FPGA.", LIB_CRIO_LINUX));

        NiFpga_Session NiSession;
        Res = NiFpga_Open(bitfile.c_str(), signature.c_str(), url.c_str(), 0, &NiSession);
        if (NiFpga_IsError(Res)) {
            NiFpga_Finalize();
            throw (CrioLibException(E_FPGA_INIT, "[%s] Failed to write bitstream. Check path, signature and IP.", LIB_CRIO_LINUX));
        }

        ctx->ai_count = 0;
        ctx->bi_count = 0;
        ctx->ao_count = 0;
        ctx->bo_count = 0;

        /* Fill in binary maps from the configuration file */
        ctx->bi_map = (void *) new bim_type;
        ctx->bi_addresses = (void *) new bm_address_type;
        ctx->bo_addresses = (void *) new bm_address_type;
        ctx->ao_addresses = (void *) new bm_address_type;
        ctx->ai_addresses = (void *) new bm_address_type;
        ctx->rt_addresses = (void *) new bm_address_type;
        ctx->scaler_name_index_map   = (void *) new bm_address_type;
        ctx->scalers = (void *) new struct scaler_ctx[MAX_SCALER_SUPPORTED_COUNT];
        TRY_THROW(parser->get_bi_maps(use_shared_memory, ctx->bi_count, (bim_type*) ctx->bi_map, (bm_address_type *)ctx->bi_addresses, (bm_address_type *)ctx->rt_addresses));
        TRY_THROW(parser->get_address_maps(use_shared_memory, ctx->bo_count, (bm_address_type *)ctx->bo_addresses, (bm_address_type *)ctx->rt_addresses, BO_ALIAS));
        TRY_THROW(parser->get_address_maps(use_shared_memory, ctx->ao_count, (bm_address_type *)ctx->ao_addresses, (bm_address_type *)ctx->rt_addresses, AO_ALIAS));
        TRY_THROW(parser->get_address_maps(use_shared_memory, ctx->ai_count, (bm_address_type *)ctx->ai_addresses, (bm_address_type *)ctx->rt_addresses, AI_ALIAS));
        TRY_THROW(parser->get_scaler_data((bm_address_type*) ctx->scaler_name_index_map, (struct scaler_ctx *)ctx->scalers));


        /* Calculate offsets if shared memory is enabled */
        if (use_shared_memory == true) {
            int rt_var_size = ((bm_address_type *)ctx->rt_addresses)->size();
            ctx->rt_variable_offsets = new uint8_t[rt_var_size];

            /* Set first offset to 0 */
            ctx->rt_variable_offsets[0] = 0;

            /* Iterate on all items of map from 0 to size-1 and calculate offset of each */
            uint8_t offset;
            for (int index = 0; index < rt_var_size-1; index ++)
            {
                try {
                    name = ((bm_address_type *)ctx->rt_addresses)->right.at(index).c_str();
                }
                catch (out_of_range) {
                    throw (CrioLibException(E_OUT_OF_RANGE, "[%s] Cannot find RT item of index <%d>.", LIB_CRIO_LINUX, index));
                }
                offset = ctx->rt_variable_offsets[index];
                ctx->rt_variable_offsets[index+1] = offset + decode_enum_size(get_rt_var_size(name));
            }

            /* open shared memory */
            TRY_THROW(open_shared_memory(shared_memory_path, &ctx->shared_memory));

        }

        /* Initialize context */
        ctx->session_open = true;
        ctx->session = (CrioSession)NiSession;
        ctx->bi_cache_valid = false;
        ctx->bi_cache_timeout = 1000;
        Res = pthread_mutex_init(&ctx->bi_mutex,NULL);
        if (Res != 0) throw (CrioLibException(E_RESOURCE_ALLOC, "[%s] Cannot create mutex.", LIB_CRIO_LINUX));
    }
    delete parser;
    return 0;
}



void crioCleanup(struct crio_context *ctx) {
    if (ctx->session_open)
    {
        NiFpga_Close(ctx->session, NiFpga_CloseAttribute_NoResetIfLastSession);
        NiFpga_Finalize();
        ctx->session_open = false;
        delete((bm_address_type *)ctx->ai_addresses);
        delete((bm_address_type *)ctx->rt_addresses);
        delete((bm_address_type *)ctx->ao_addresses);
        delete((bm_address_type *)ctx->bo_addresses);
        delete((bm_address_type *)ctx->bi_addresses);
        delete((bm_address_type *)ctx->scaler_name_index_map);
        delete((struct scaler_ctx*)ctx->scalers);

        delete ctx->rt_variable_offsets;
        delete((bim_type * )ctx->bi_map);
        pthread_mutex_destroy(&ctx->bi_mutex);
    }
}
