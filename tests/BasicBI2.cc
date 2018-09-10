#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context ctx;
    unsigned Size;
    auto Res = CrioSetup(&ctx);
    const char *Name;
    bool Item;
    assert(Res == 0);
    cout << "Binary input values:" << "\n";
    CrioGetBIArraySize(&ctx, &Size);

    for (unsigned I = 0; I < Size; I++) {
        Res = CrioGetBIArrayItemName(&ctx, I, &Name);
        assert(Res == 0);
        Res = CrioGetBIArrayItemByIndex(&ctx, &Item, I);
        assert(Res == 0);

        cout << Name << ": " << Item  << '\n';
    }
    cout << '\n';


    CrioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
