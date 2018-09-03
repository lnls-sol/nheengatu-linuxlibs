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
        bool Item;
	    Res = CrioGetBIArrayItemName(Session, I, &Name);
	    Res = CrioGetBIArrayItemByIndex(Session, &Item, I);
	    assert(Res == 0);

	    cout << Name << ": " << Item  << '\n';
    }
    cout << '\n';

    CrioCleanup(Session);
    cout << "FINISHED.\n";

    return 0;
}
