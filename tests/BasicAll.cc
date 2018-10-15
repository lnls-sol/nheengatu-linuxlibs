#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    double ai_val;
    unsigned Size;
    char cfg[] = "cfg/cfg.ini";
    bool Item;
    double ao_val;
    srand(time(0));
    string BIs[] = {
        "Mod3/DIO0",
        "Mod3/DIO1",
        "Mod3/DIO2",
        "Mod3/DIO3",
        "Mod3/DIO4",
        "Mod3/DIO5",
        "Mod3/DIO6",
        "Mod3/DIO7",
        "Mod3/DIO8",
        "Mod3/DIO9",
        "Mod3/DIO10",
        "Mod3/DIO11",
        "Mod3/DIO12",
        "Mod3/DIO13",
        "Mod3/DIO14",
        "Mod3/DIO15",
        "Mod3/DIO16",
        "Mod3/DIO17",
        "Mod3/DIO18",
        "Mod3/DIO19",
        "Mod3/DIO20",
        "Mod3/DIO21",
        "Mod3/DIO22",
        "Mod3/DIO23",
        "Mod3/DIO24",
        "Mod3/DIO25",
        "Mod3/DIO26",
        "Mod3/DIO27",
        "Mod3/DIO28",
        "Mod3/DIO29",
        "Mod3/DIO30",
        "Mod3/DIO31",
        "RT_BOL_BITest"};

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
                    "Mod2/DIO7",
                    "RT_BOL_BO0"};

    string AIs[] = {"Mod4/AI0", "Mod4/AI1", "Mod4/AI2", "Mod4/AI3",
                    "Mod6/TC0", "Mod6/TC1", "Mod6/TC2", "Mod6/TC3",
                    "Mod7/AI0", "Mod7/AI1", "Mod7/AI2", "Mod7/AI3",
                    "Mod8/AI0", "Mod8/AI1", "Mod8/AI2", "Mod8/AI3",
                    "RT_DBL_AI0", "RT_SGL_AI1", "RT_I64_AI2", "RT_I32_AI3",
                    "RT_I16_AI4", "RT_I08_AI5", "RT_U64_AI6", "RT_U32_AI7",
                    "RT_U16_AI8", "RT_U08_AI9"};


    string AOs[] = {"Mod5/AO0", "Mod5/AO1", "Mod5/AO2", "Mod5/AO3",
                    "RT_DBL_AO1", "RT_SGL_AO2", "RT_I64_AO3", "RT_I32_AO4",
                   "RT_I16_AO5", "RT_I08_AO6", "RT_U64_AO7", "RT_U32_AO8",
                   "RT_U16_AO9", "RT_U08_AO10"};


    auto Res = crioSetup(ctx, cfg);
    switch (Res)
    {
        case 0 : cout << "crioSetup executed successfully\n"; break;
        case -1 : cout << "Failed in reading ini file\n"; return -1; break;
        case -2 : cout << "Failed in initializing FPGA\n"; return -1; break;
        case -3 : cout << "Failed in initializing Shared Memory\n"; return -1; break;
        default: break;
    }

    // Seems like the FPGA needs 1 second to start bringing data available on the ports.
    // This value was obtained emperically.
    sleep(1);

    /* BI */
    crioGetBIArraySize(ctx, &Size);
    cout << "Binary inputs found:" << Size << endl;

    for (unsigned I = 0; I < Size; I++) {
        Res = crioGetBIArrayItemByName(ctx, &Item, BIs[I].c_str());
        switch (Res)
        {
            case -1 : cout << "Index " << I << " does not exist\n"; return -1; break;
            case -2 : cout << "CRIO session not open\n"; return -1; break;
            default: break;
        }
        cout << BIs[I].c_str() << ": " << Item  << endl;
    }
    cout << endl;

    /* BO */
    crioGetBOArraySize(ctx, &Size);
    cout << "Binary outputs found:" << Size << endl;
    Item = rand() % 2;
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
        cout << AOs[x].c_str() << "->" << static_cast<int64_t>(ao_val) << endl;
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

        cout << AIs[x].c_str() << "->" << static_cast<int64_t>(ai_val) << endl;
    }

    crioCleanup(ctx);
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
