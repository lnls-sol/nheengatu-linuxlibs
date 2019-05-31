#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>

#define WF_SIZE 3
int main(void) {
    struct crio_context * ctx = new struct crio_context;


    char cfg[] = "cfg/cfg_waveform.ini";


    cout.precision(64);
    srand(time(0));
    uint32_t size;
    string waveforms[WF_SIZE] = {"RT_WF0",
                                 "RT_WF1",
                                 "RT_WF2"};

    float    wf0[5];
    uint32_t wf1[113];
    double   wf2[4];


    printLibVersion();
    TRY_THROW(crioSetup(ctx, cfg));


    // Seems like the FPGA needs 1 second to start bringing data available on the ports.
    // This value was obtained emperically.
    sleep(1);

    /* Waveform */

    //crioGetWaveformItem(struct crio_context *ctx, const char *name, void *array)
    TRY_SILENT_CONT(crioGetWaveformItem(ctx, waveforms[0].c_str(), wf0, &size));
    TRY_SILENT_CONT(crioGetWaveformItem(ctx, waveforms[1].c_str(), wf1, &size));
    TRY_SILENT_CONT(crioGetWaveformItem(ctx, waveforms[2].c_str(), wf2, &size));

    for (uint i =0 ; i < 5; i++)
        cout << waveforms[0].c_str() << "[" << i << "]: " << wf0[i]  << endl;

    for (uint i =0 ; i < 113; i++)
        cout << waveforms[1].c_str() << "[" << i << "]: " << wf1[i]  << endl;

    for (uint i =0 ; i < 4; i++)
        cout << waveforms[2].c_str() << "[" << i << "]: " << wf2[i]  << endl;



    TRY_SILENT(crioCleanup(ctx));
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
