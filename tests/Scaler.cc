#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    char cfg[] = "cfg/scaler.ini";
    char name[] = "SCALER0";
    uint32_t counters[64];
    bool done = false;
    TRY_SILENT(crioSetup(ctx, cfg));
    sleep(1);
    TRY_SILENT(crioSetScalerReset(ctx, name));
    TRY_SILENT(crioSetScalerGates(ctx, name, 0, true));
    TRY_SILENT(crioSetScalerPresets(ctx, name, 0, 100000));
    TRY_SILENT(crioSetScalerArm(ctx, name, true, true));
    while(done == false)
    {
        crioGetScalerDone(ctx, name, &done);
        crioGetScalerCounters(ctx, name, counters);
        for (int i = 0 ; i < 1; i++)
            cout << "Counter " << i << " : " << counters[i] << endl;
    }
    cout << "Done!\n";
    delete(ctx);
    return 0;
}
