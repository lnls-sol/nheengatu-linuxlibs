#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>

int main(void) {
    struct crio_context ctx;
    auto Res = crioSetup(&ctx);
    assert(Res == 0);

    cout << "Session opened.\n";

    crioCleanup(&ctx);

    cout << "Session closed.\n";

    return 0;
}
