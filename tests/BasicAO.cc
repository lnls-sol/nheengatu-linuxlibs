#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context ctx;
    unsigned Size;
    string AOs[] = {"Mod5/AO0", "Mod5/AO1"};
    float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
    char cfg[] = "cfg/ao_cfg.ini";
    auto Res = crioSetup(&ctx, cfg);
    switch (Res)
    {
        case -1 : cout << "Failed in reading ini file\n"; break;
        case -2 : cout << "Failed in initializing FPGA\n"; break;
        default: break;
    }

    crioGetBOArraySize(&ctx, &Size);

    while (1)
    {
        Res = crioSetAOItem(&ctx, AOs[0].c_str(), x);
        if (Res == -1)
            cout << "Query returned NULL for AO address of name: " << AOs[0].c_str() << endl;
        Res = crioSetAOItem(&ctx, AOs[1].c_str(), 10-x);
        if (Res == -1)
            cout << "Query returned NULL for AO address of name: " << AOs[1].c_str() << endl;
        assert(Res == 0);
        cout << AOs[0].c_str() << "->" << x << endl;
        cout << AOs[1].c_str() << "->" << 10-x << endl;
        getchar();
        x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
    }

    crioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
