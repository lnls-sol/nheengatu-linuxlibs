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
    string waveforms[WF_SIZE] = {"WAVEFORM0",
                                 "WAVEFORM1",
                                 "WAVEFORM2"};

    uint32_t wf0[3];
    uint64_t wf1[3];
    float    wf2[3];

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

    cout << waveforms[0].c_str() << "[0]: " << wf0[0]  << endl;
    cout << waveforms[0].c_str() << "[1]: " << wf0[1]  << endl;
    cout << waveforms[0].c_str() << "[2]: " << wf0[2]  << endl;

    cout << waveforms[1].c_str() << "[0]: " << wf1[0]  << endl;
    cout << waveforms[1].c_str() << "[1]: " << wf1[1]  << endl;
    cout << waveforms[1].c_str() << "[2]: " << wf1[2]  << endl;

    cout << waveforms[2].c_str() << "[0]: " << wf2[0]  << endl;
    cout << waveforms[2].c_str() << "[1]: " << wf2[1]  << endl;
    cout << waveforms[2].c_str() << "[2]: " << wf2[2]  << endl;


    TRY_SILENT(crioCleanup(ctx));
    cout << "FINISHED.\n";
    delete(ctx);
    return 0;
}
