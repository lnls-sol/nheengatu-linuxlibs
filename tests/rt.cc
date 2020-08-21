#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    unsigned Size;
    char cfg[] = "cfg/cfgstring.ini";
    double ao_val, ai_val;
    cout.precision(64);
    srand(time(0));
    char stringi[41];
    char stringo[41] = "012345678901234567890123456789ABDCEFGHIJ";

    string AO[] = {"RT_DBL_AO_DBL1", "RT_I64_AO_i64_3", "RT_U64_AO_u64_7"};
    string STRINGIN[] = {"RT_STI_str2", "RT_STI_stringin6"};
    string STRINGOUT[] = {"RT_STO_string4"};
    string AI[] = {"RT_U32_AI_u32_5"};

    printLibVersion();
    TRY_THROW(crioSetup(ctx, cfg));


    // Seems like the FPGA needs 1 second to start bringing data available on the ports.
    // This value was obtained emperically.
    sleep(1);


    TRY_SILENT(crioGetAOArraySize(ctx, &Size));
    cout << "AOs found in cfg file:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        ao_val = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
        TRY_SILENT_CONT(crioSetAOItem(ctx, AO[x].c_str(), ao_val));
        if (true == is_rt_var(AO[x].c_str())) {
            switch (get_rt_var_size(AO[x].c_str()))
            {
                case DBL: case SGL : cout << AO[x].c_str() << "->" << ao_val << endl; break;
                case U64: case U32: case U16: case U08: cout << AO[x].c_str() << "->" << static_cast<uint64_t>(ao_val) << endl; break;
                case I64: case I32: case I16: case I08: cout << AO[x].c_str() << "->" << static_cast<int64_t>(ao_val) << endl; break;
            default: cout << "Found unknown RT variable size in AO \n";
            }
        }
        else
            cout << AO[x].c_str() << "->" << ao_val << endl;
    }
    cout << endl;

    TRY_SILENT(crioGetAIArraySize(ctx, &Size));
    cout << "AIs found in cfg file:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        TRY_SILENT_CONT(crioGetAIItem(ctx, AI[x].c_str(), ai_val));
        cout << AI[x].c_str() << "->" << ai_val << endl;
    }

    TRY_SILENT(crioGetSTRINGINArraySize(ctx, &Size));
    cout << "STRINGINs found in cfg file:" << Size << endl;
    for (uint x = 0; x < Size; x++)
    {
        TRY_SILENT_CONT(crioGetSTRINGINItem(ctx, STRINGIN[x].c_str(), stringi));
        cout << STRINGIN[x].c_str() << "->" << stringi << endl;
    }


    TRY_SILENT(crioGetSTRINGOUTArraySize(ctx, &Size));
    cout << "STRINGOUTs found in cfg file:" << Size << endl;
    TRY_SILENT_CONT(crioSetSTRINGOUTItem(ctx, STRINGOUT[0].c_str(), stringo));


    TRY_SILENT(crioCleanup(ctx));
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
