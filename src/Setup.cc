#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"

int CrioSetup(struct crio_context *ctx) {
    string ip = "";
    string path = "";
    string url = "";
    string bitfile = "";
    string fileName = "";
    string signature = "";

    /* Read cfg file */
    cfg_parser parser;
    bm_type * myBimap = new bm_type;
    ctx->bi_map = (void *) myBimap;

    bm_address_type * bi_addresses = new bm_address_type;
    ctx->bi_addresses = (void *) bi_addresses;

    /* Fill in map */
    parser.get_bimaps((bm_type*) ctx->bi_map, (bm_address_type *)ctx->bi_addresses);

    /* Get settings from configuration file */
    parser.get_settings(ip, path, fileName, signature);

    url = "rio://" + ip + "/RIO0";
    bitfile = path + "/" + fileName;

    auto Res = NiFpga_Initialize();
    if (NiFpga_IsError(Res)) return -1;

    NiFpga_Session NiSession;
    Res = NiFpga_Open(bitfile.c_str(), signature.c_str(), url.c_str(), 0, &NiSession);
    if (NiFpga_IsError(Res)) {
        NiFpga_Finalize();
        return -2;
    }

    /* Initialize context */
    ctx->session_open = true;
    ctx->session = (CrioSession)NiSession;
    Res = pthread_mutex_init(&ctx->bi_mutex,NULL);
    if (Res != 0)
    {
        perror("pthread_mutex_init");
        return -1;
    }

    return 0;
}

void CrioCleanup(struct crio_context *ctx) {
    NiFpga_Close(ctx->session, NiFpga_CloseAttribute_NoResetIfLastSession);
    NiFpga_Finalize();
    ctx->session_open = false;
    delete(ctx->bi_addresses);
    delete(ctx->bi_map);
    pthread_mutex_destroy(&ctx->bi_mutex);
}
