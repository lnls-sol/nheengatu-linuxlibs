#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context * ctx = new struct crio_context;
    float ai_val;
    unsigned Size;
    char cfg[] = "cfg/cfg.ini";
    const char *Name;
    bool Item;
    uint ao_val;

    string BOs[] = {"Mod1/DIO0",
                    "Mod1/DIO1",
                    "Mod1/DIO2",
                    "Mod1/DIO3",
                    "Mod1/DIO4",
                    "Mod1/DIO5",
                    "Mod1/DIO6",
                    "Mod1/DIO7",
                    "Mod2/DIO0",
                    "Mod2/DIO1",
                    "Mod2/DIO2",
                    "Mod2/DIO3",
                    "Mod2/DIO4",
                    "Mod2/DIO5",
                    "Mod2/DIO6",
                    "Mod2/DIO7"};

    string AIs[] = {"Mod4/AI0", "Mod4/AI1", "Mod4/AI2", "Mod4/AI3",
                    "Mod6/TC0", "Mod6/TC1", "Mod6/TC2", "Mod6/TC3",
                    "Mod7/AI0", "Mod7/AI1", "Mod7/AI2", "Mod7/AI3",
                    "Mod8/AI0", "Mod8/AI1", "Mod8/AI2", "Mod8/AI3"};

    string AOs[] = {"Mod5/AO0", "Mod5/AO1", "Mod5/AO2", "Mod5/AO3"};


    auto Res = crioSetup(ctx, cfg);
    switch (Res)
    {
        case 0 : cout << "crioSetup executed successfully\n"; break;
        case -1 : cout << "Failed in reading ini file\n"; return -1; break;
        case -2 : cout << "Failed in initializing FPGA\n"; return -1; break;
        default: break;
    }

    // Seems like the FPGA needs 1 second to start bringing data available on the ports.
    // This value was obtained emperically.
    sleep(1);

    /* BI */
    crioGetBIArraySize(ctx, &Size);
    cout << "Binary inputs found:" << Size << endl;

    for (unsigned I = 0; I < Size; I++) {
        Res = crioGetBIArrayItemName(ctx, I, &Name);
        switch (Res)
        {
            case -1 : cout << "Index " << I << " does not exist\n"; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }
        Res = crioGetBIArrayItemByIndex(ctx, &Item, I);
        switch (Res)
        {
            case -1 : cout << "Index " << I << " does not exist\n"; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }
        cout << Name << ": " << Item  << endl;
    }
    cout << endl;

    /* BO */
    crioGetBOArraySize(ctx, &Size);
    cout << "Binary outputs found:" << Size << endl;
    Item = false;
    for (uint x = 0; x < Size; x++)
    {
        Res = crioSetBOItem(ctx, BOs[x].c_str(), Item);
        switch (Res)
        {
            case -1 : cout << "Item " << BOs[x].c_str() << " does not exist\n"; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }
        cout << BOs[x].c_str() << "->" << Item << endl;
        Item = !Item;
        assert(Res == 0);
    }
    cout << endl;

    /* AI */
    crioGetAIArraySize(ctx, &Size);
    cout << "Analog inputs found:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        Res = crioGetAIItem(ctx, AIs[x].c_str(), ai_val);
        switch (Res)
        {
            case -1 : cout << "Query returned NULL for AI address of name: " << AIs[x].c_str() << endl; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }

        cout << AIs[x].c_str() << "->" << ai_val << endl;
    }
    cout << endl;

    /* AO */
    crioGetAOArraySize(ctx, &Size);
    cout << "Analog outputs found:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        ao_val = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
        Res = crioSetAOItem(ctx, AOs[x].c_str(), ao_val);
        switch (Res)
        {
            case -1 : cout << "Query returned NULL for AO address of name: " << AOs[x].c_str() << endl; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }
        cout << AOs[x].c_str() << "->" << ao_val << endl;
    }
    cout << endl;
    crioCleanup(ctx);
    cout << "FINISHED.\n";

    delete(ctx);
    return 0;
}
