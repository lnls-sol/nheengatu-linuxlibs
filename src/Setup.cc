#include "Common.h"
#include "CrioLinux.h"

static const string BITFILE_PATH = BUILD_CRIO_LINUX_LIBDIR;

// FIXME: Make configurable
static const string CRIO_URI = "rio://NI-cRIO-9039-01BF96B4-DCM/RIO0";

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
    return 0;
}

void CrioCleanup(CrioSession Session) {
    NiFpga_Close(Session, NiFpga_CloseAttribute_NoResetIfLastSession);
    NiFpga_Finalize();
}
