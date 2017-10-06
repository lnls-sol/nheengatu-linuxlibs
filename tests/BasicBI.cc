#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>

int main(void) {
    CrioSession Session;
    auto Res = CrioSetup(&Session);
    assert(Res == 0);

    uint64_t Output;
    Res = CrioReadBIArray(Session, &Output);
    assert(Res == 0);

    cout << "Binary input values:\n";
    for (unsigned I = 0; I < CRIO_BI_ARRAY_COUNT; I++) {
	    const char *Name;
	    Res = CrioGetBIArrayItemName(Session, I, &Name);
	    assert(Res == 0);

	    cout << Name << ": " << (bool)(Output & (1<<I)) << '\n';
    }
    cout << '\n';

    CrioCleanup(Session);
    cout << "FINISHED.\n";

    return 0;
}
