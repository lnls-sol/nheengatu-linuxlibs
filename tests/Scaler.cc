#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

int main(void) {
    struct crio_context * ctx = new struct crio_context;
    char cfg[] = "cfg/scaler.ini";
    char name_digital[] = "SCALER_DIGITAL";
    uint32_t counters[2];
    bool done = false;
    TRY_SILENT(crioSetup(ctx, cfg));
    TRY_SILENT(crioSetScalerReset(ctx, name_digital));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name_digital, 0, 100000));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name_digital, 1, 2500));
    TRY_SILENT(crioSetScalerArm(ctx, name_digital, true, true));
    while(done == false)
    {
        crioGetScalerDone(ctx, name_digital, &done);
        crioGetScalerCounters(ctx, name_digital, counters);
        for (int i = 0 ; i < 2; i++)
            cout << "Counter " << i << " : " << counters[i] << endl;
    }
    cout << "Done digital!\n";

    char name_analog[] = "SCALER_ANALOG";
    done = false;
    TRY_SILENT(crioSetScalerReset(ctx, name_analog));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name_analog, 0, 100000));
    TRY_SILENT(crioSetScalerPresetsGates(ctx, name_analog, 1, 25000));
    TRY_SILENT(crioSetScalerArm(ctx, name_analog, true, true));
    while(done == false)
    {
        crioGetScalerDone(ctx, name_analog, &done);
        crioGetScalerCounters(ctx, name_analog, counters);
        for (int i = 0 ; i < 2; i++)
            cout << "Counter " << i << " : " << counters[i] << endl;
    }
    cout << "Done analog!\n";
    crioCleanup(ctx);
    delete(ctx);
    return 0;
}
