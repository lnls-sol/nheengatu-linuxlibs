#include "Common.h"
#include "CrioLinux.h"

struct crio_ctx ctx;

static const string BITFILE_PATH = BUILD_CRIO_LINUX_LIBDIR;

// FIXME: Make configurable
static const string CRIO_URI = "rio://127.0.0.1/RIO0";

int CrioSetup(CrioSession *Session) {
    auto Res = NiFpga_Initialize();
    if (NiFpga_IsError(Res)) return -1;

    auto Path = BITFILE_PATH + NiFpga_CrioLinux_Bitfile;
    NiFpga_Session NiSession;
    Res = NiFpga_Open(Path.c_str(), NiFpga_CrioLinux_Signature, CRIO_URI.c_str(), 0, &NiSession);
    if (NiFpga_IsError(Res)) {
        NiFpga_Finalize();
        return -2;
    }

    *Session = (CrioSession)NiSession;

    /* Initialize context */
    ctx.session = *Session;
    ctx.session_open = true;
    Res = pthread_mutex_init(&ctx.bi_mutex,NULL);
    if (Res != 0)
    {
        perror("pthread_mutex_init");
        return -1;
    }

    return 0;
}

void CrioCleanup(CrioSession Session) {
    NiFpga_Close(Session, NiFpga_CloseAttribute_NoResetIfLastSession);
    NiFpga_Finalize();
}
