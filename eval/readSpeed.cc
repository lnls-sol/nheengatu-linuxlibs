#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <time.h>

int main(void) {
    struct crio_context ctx;
    clock_t start, stop, diff;
    NiFpga_Bool Output;
    uint64_t reads=10000;
    char cfgfile[] = "cfg/bi32array_cfg.ini";
    NiFpga_Bool boolArray[32];
    auto Res = crioSetup(&ctx, cfgfile);
    assert(Res == 0);

    start = clock();
    for (uint64_t i = 0; i < reads; i++)
        Res = NiFpga_ReadBool(ctx.session, 0x18006, &Output );
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu reads (%f read/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));


    start = clock();
    for (uint64_t i = 0; i < reads; i++)
        Res = NiFpga_ReadArrayBool(ctx.session, 0x18000, boolArray , 32);



    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu reads (%f read/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));


    start = clock();
    for (uint64_t i = 0; i < reads; i++)
	    Res = Output;
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu assignments (%f assignments/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));
    
    crioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
