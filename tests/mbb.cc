#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    unsigned Size;
    char cfg[] = "cfg/cfg_mbb.ini";
    double Item;
    cout.precision(64);
    srand(time(0));


    string MBBIs[] = {"mbbi_ind"};


    string MBBOs[] = {"mbbo_ctl"};

    printLibVersion();
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


    TRY_SILENT(crioGetMBBIArraySize(ctx, &Size));
    cout << "MBBIs found:" << Size << endl;

    TRY_SILENT(crioGetMBBOArraySize(ctx, &Size));
    cout << "MBBOs found:" << Size << endl;

    for (unsigned I = 0; I < 3; I++) {
        TRY_SILENT_CONT(crioSetMBBOItem(ctx, MBBOs[0].c_str(), I));
        cout << MBBOs[0].c_str() << "->" << I << endl;
        TRY_SILENT_CONT(crioGetMBBIItem(ctx, MBBIs[0].c_str(), Item));
        cout << MBBIs[0].c_str() << "->" << Item  << endl;

    }
    cout << endl;

    TRY_SILENT(crioCleanup(ctx));
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
