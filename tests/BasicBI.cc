#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context ctx;
    unsigned Size;
    char cfg[] = "cfg/bi_cfg.ini";
    auto Res = crioSetup(&ctx, cfg);
    const char *Name;
    bool Item;
    assert(Res == 0);
    cout << "Binary input values:" << "\n";
    crioGetBIArraySize(&ctx, &Size);

    for (unsigned I = 0; I < Size; I++) {
        Res = crioGetBIArrayItemName(&ctx, I, &Name);
        assert(Res == 0);
        Res = crioGetBIArrayItemByIndex(&ctx, &Item, I);
        assert(Res == 0);

        cout << Name << ": " << Item  << '\n';
    }
    cout << '\n';


    crioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
