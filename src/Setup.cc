#include "Common.h"
#include "CrioLinux.h"
#include "cfg_parser.h"

static const string BITFILE_PATH = BUILD_CRIO_LINUX_LIBDIR;

// FIXME: Make configurable
static const string CRIO_URI = "rio://127.0.0.1/RIO0";



int CrioSetup(struct crio_context *ctx) {
    auto Res = NiFpga_Initialize();
    if (NiFpga_IsError(Res)) return -1;

    auto Path = BITFILE_PATH + NiFpga_CrioLinux_Bitfile;
    NiFpga_Session NiSession;
    Res = NiFpga_Open(Path.c_str(), NiFpga_CrioLinux_Signature, CRIO_URI.c_str(), 0, &NiSession);
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

    /* Read cfg file */
    cfg_parser parser;
    bm_type * myBimap = new bm_type;
    ctx->bi_map = (void *) myBimap;

    /* Fill in map */
    parser.get_bimap((bm_type *)ctx->bi_map);

    /* Print bimap
    for( bm_type::const_iterator iter = myBimap->begin(), iend = myBimap->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl;*/

    return 0;
}

void CrioCleanup(struct crio_context *ctx) {
    NiFpga_Close(ctx->session, NiFpga_CloseAttribute_NoResetIfLastSession);
    NiFpga_Finalize();
    ctx->session_open = false;
    pthread_mutex_destroy(&ctx->bi_mutex);
}
