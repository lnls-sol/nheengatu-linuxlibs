#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"

int crioSetup(struct crio_context *ctx, char *cfgfile) {
    string ip = "";
    string path = "";
    string url = "";
    string bitfile = "";
    string fileName = "";
    string signature = "";

    if (!ctx->session_open)
    {
        /* Read cfg file */
        cfg_parser parser(cfgfile);

        /* Get settings from configuration file */
        parser.get_settings(ip, path, fileName, signature);

        url = "rio://" + ip + "/RIO0";
        bitfile = path + "/" + fileName;

        /* Setting up CRIO */
        auto Res = NiFpga_Initialize();
        if (NiFpga_IsError(Res)) return -1;

        NiFpga_Session NiSession;
        Res = NiFpga_Open(bitfile.c_str(), signature.c_str(), url.c_str(), 0, &NiSession);
        if (NiFpga_IsError(Res)) {
            NiFpga_Finalize();
            return -2;
        }

        /* Fill in binary maps from the configuration file */
        ctx->bi_map = (void *) new bim_type;
        ctx->bi_addresses = (void *) new bm_address_type;
        ctx->bo_addresses = (void *) new bm_address_type;
        ctx->ao_addresses = (void *) new bm_address_type;

        Res = parser.get_bi_maps((bim_type*) ctx->bi_map, (bm_address_type *)ctx->bi_addresses);
        if (Res != 0)  return -1;

        Res = parser.get_output_maps((bm_address_type *)ctx->bo_addresses, BO_ALIAS);
        if (Res != 0)  return -1;

        Res = parser.get_output_maps((bm_address_type *)ctx->ao_addresses, AO_ALIAS);
        if (Res != 0)  return -1;

        /* Initialize context */
        ctx->session_open = true;
        ctx->session = (CrioSession)NiSession;
        ctx->bi_cache_valid = false;
        Res = pthread_mutex_init(&ctx->bi_mutex,NULL);
        if (Res != 0)
        {
            perror("pthread_mutex_init");
            return -1;
        }
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
        delete((bm_address_type *)ctx->bi_addresses);
        delete((bim_type * )ctx->bi_map);
        pthread_mutex_destroy(&ctx->bi_mutex);
    }
}
