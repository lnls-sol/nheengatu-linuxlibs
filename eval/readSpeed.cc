#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>
#include <time.h>

int main(void) {
    CrioSession Session;
    clock_t start, stop, diff;
    uint64_t Output;
    uint64_t reads=10000;
    auto Res = CrioSetup(&Session);
    assert(Res == 0);

    start = clock();
    for (uint64_t i = 0; i < reads; i++)
	    Res = CrioReadBIArray(Session, &Output);
    stop = clock();
    diff = stop - start;
    printf ("It took %ld clicks (%f seconds) for %lu reads (%f read/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));


    start = clock();
    for (uint64_t i = 0; i < reads; i++)
	    Res = Output;
    stop = clock();
    diff = stop - start;
    printf ("It took %ld clicks (%f seconds) for %lu assignments (%f assignments/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));
    
    CrioCleanup(Session);
    cout << "FINISHED.\n";

    return 0;
}
