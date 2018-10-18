#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"
#include "utils.h"
#include "rt_var_handler.h"

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
    if (ctx->session_open == false)
    {
        /* Read cfg file */
        cfg_parser parser(cfgfile);

        /* Get settings from configuration file */
        auto Res = parser.get_settings(ip, path, fileName, signature, use_shared_memory, shared_memory_path);
        if (Res != 0)  return -1;

        url = "rio://" + ip + "/RIO0";
        bitfile = path + "/" + fileName;

        /* Setting up CRIO */
        Res = NiFpga_Initialize();
        if (NiFpga_IsError(Res)) return -1;

        NiFpga_Session NiSession;
        Res = NiFpga_Open(bitfile.c_str(), signature.c_str(), url.c_str(), 0, &NiSession);
        if (NiFpga_IsError(Res)) {
            NiFpga_Finalize();
            return -2;
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

        Res = parser.get_bi_maps(ctx->bi_count, (bim_type*) ctx->bi_map, (bm_address_type *)ctx->bi_addresses, (bm_address_type *)ctx->rt_addresses);
        if (Res != 0)  return -1;

        Res = parser.get_address_maps(ctx->bo_count, (bm_address_type *)ctx->bo_addresses, (bm_address_type *)ctx->rt_addresses, BO_ALIAS);
        if (Res != 0)  return -1;

        Res = parser.get_address_maps(ctx->ao_count, (bm_address_type *)ctx->ao_addresses, (bm_address_type *)ctx->rt_addresses, AO_ALIAS);
        if (Res != 0)  return -1;

        Res = parser.get_address_maps(ctx->ai_count, (bm_address_type *)ctx->ai_addresses, (bm_address_type *)ctx->rt_addresses, AI_ALIAS);
        if (Res != 0)  return -1;

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
                    return -1;
                }
                offset = ctx->rt_variable_offsets[index];
                ctx->rt_variable_offsets[index+1] = offset + decode_enum_size(get_rt_var_size(name));
            }

            /* open shared memory */
            Res = open_shared_memory(shared_memory_path, &ctx->shared_memory);
            if (Res != 0)  return -3;
        }

        /* Initialize context */
        ctx->session_open = true;
        ctx->session = (CrioSession)NiSession;
        ctx->bi_cache_valid = false;
        Res = pthread_mutex_init(&ctx->bi_mutex,NULL);
        if (Res != 0) return -4;
    }
    return 0;
}

void crioCleanup(struct crio_context *ctx) {
    if (ctx->session_open)
    {
        NiFpga_Close(ctx->session, NiFpga_CloseAttribute_NoResetIfLastSession);
        NiFpga_Finalize();
        ctx->session_open = false;
        /* Fixme: crioCleanup immediately after crioSetup causes segmentation fault. */
        delete((bm_address_type *)ctx->ai_addresses);
        delete((bm_address_type *)ctx->rt_addresses);
        delete((bm_address_type *)ctx->ao_addresses);
        delete((bm_address_type *)ctx->bo_addresses);
        delete((bm_address_type *)ctx->bi_addresses);
        delete ctx->rt_variable_offsets;
        delete((bim_type * )ctx->bi_map);
        pthread_mutex_destroy(&ctx->bi_mutex);
    }
}
