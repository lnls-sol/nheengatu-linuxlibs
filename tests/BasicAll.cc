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
                    "RT_BOL_BO0",
                    "Mod3/DIO31"};

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


    TRY_THROW(crioSetup(ctx, cfg));
    /*bool x = false;
    while(1)
    {
        Res = crioSetBOItem(ctx, "Mod3/DIO31", x);
        x = !x;
        usleep(0.1 * 1000000);
        printf("Altered x to %u\n", x);
    }*/

    // Seems like the FPGA needs 1 second to start bringing data available on the ports.
    // This value was obtained emperically.
    sleep(1);

    /* BI */
    TRY_SILENT(crioGetBIArraySize(ctx, &Size));
    cout << "Binary inputs found:" << Size << endl;

    for (unsigned I = 0; I < Size; I++) {
        TRY_SILENT_CONT(crioGetBIArrayItemByName(ctx, &Item, BIs[I].c_str()));
        cout << BIs[I].c_str() << ": " << Item  << endl;
    }
    cout << endl;

    /* BO */
    TRY_SILENT(crioGetBOArraySize(ctx, &Size));
    cout << "Binary outputs found:" << Size << endl;
    Item = rand() % 2;
    for (uint x = 0; x < Size; x++)
    {
        TRY_SILENT_CONT(crioSetBOItem(ctx, BOs[x].c_str(), Item));
        cout << BOs[x].c_str() << "->" << Item << endl;
        Item = !Item;
    }


    cout << endl;

    /* AO */
    TRY_SILENT(crioGetAOArraySize(ctx, &Size));
    cout << "Analog outputs found:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        ao_val = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
        TRY_SILENT_CONT(crioSetAOItem(ctx, AOs[x].c_str(), ao_val));
        cout << AOs[x].c_str() << "->" << ao_val << endl;
    }
    cout << endl;


    /* AI */
    TRY_SILENT(crioGetAIArraySize(ctx, &Size));
    cout << "Analog inputs found:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        TRY_SILENT_CONT(crioGetAIItem(ctx, AIs[x].c_str(), ai_val));
        if (true == is_rt_var(AIs[x].c_str())) {
            if (U64 == get_rt_var_size(AIs[x].c_str()))
                cout << AIs[x].c_str() << "->" << static_cast<int64_t>(ai_val) << endl;
            else
                cout << AIs[x].c_str() << "->" << ai_val << endl;
        }
        else
            cout << AIs[x].c_str() << "->" << ai_val << endl;
    }

    TRY_SILENT(crioCleanup(ctx));
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
