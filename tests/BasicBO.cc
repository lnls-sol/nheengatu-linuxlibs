#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>



int main(void) {
    struct crio_context ctx;
    unsigned Size;
    string BOs[] = {"Mod3/DIO31", "Mod3/DIO30"};
    bool x = true;
    auto Res = crioSetup(&ctx);
    assert(Res == 0);

    crioGetBOArraySize(&ctx, &Size);

    while (1)
    {
        Res = crioSetBOItem(&ctx, BOs[0].c_str(), x);
        Res = crioSetBOItem(&ctx, BOs[1].c_str(), !x);
        cout << BOs[0].c_str() << "->" << x << endl;
        cout << BOs[1].c_str() << "->" << !x << endl;
        sleep(1);
        x = !x;
        assert(Res == 0);
    }

    crioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
