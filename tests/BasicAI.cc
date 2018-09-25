#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context ctx;
    unsigned Size;
    string AIs[] = {"Mod8/AI0", "Mod8/AI1", "Mod8/AI2", "Mod8/AI3"};
    char cfg[] = "cfg/ai_cfg.ini";
    auto Res = crioSetup(&ctx, cfg);
    float inputs[4];
    switch (Res)
    {
        case -1 : cout << "Failed in reading ini file\n"; break;
        case -2 : cout << "Failed in initializing FPGA\n"; break;
        default: break;
    }

    crioGetAIArraySize(&ctx, &Size);

    while (1)
    {
        for (int x = 0; x < 4; x++)
        {
            Res = crioGetAIItem(&ctx, AIs[x].c_str(), inputs[x]);
            switch (Res)
            {
                case -1 : cout << "Query returned NULL for AI address of name: " << AIs[x].c_str() << endl; return -1; break;
                case -2 : cout << "CRIO session not open\n"; return -1; break;
                default: break;
            }
            cout << AIs[x].c_str() << "->" << inputs[x] << endl;
        }
        sleep(1);
    }

    crioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
