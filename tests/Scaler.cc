#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    char cfg[] = "cfg/scaler.ini";
    char name[] = "NI:SCALER0";
    uint32_t counters[64];
    bool done = false;
    TRY_SILENT(crioSetup(ctx, cfg));
    TRY_SILENT(crioSetScalerReset(ctx, name));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name, 0, 100000));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name, 1, 2500));
    TRY_SILENT(crioSetScalerArm(ctx, name, true, true));
    while(done == false)
    {
        crioGetScalerDone(ctx, name, &done);
        crioGetScalerCounters(ctx, name, counters);
        for (int i = 0 ; i < 2; i++)
            cout << "Counter " << i << " : " << counters[i] << endl;
    }
    cout << "Done!\n";
    delete(ctx);
    return 0;
}
