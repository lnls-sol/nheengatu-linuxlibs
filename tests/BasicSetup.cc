#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>

int main(void) {
    struct crio_context ctx;
    auto Res = CrioSetup(&ctx);
    assert(Res == 0);

    cout << "Session opened.\n";

    CrioCleanup(&ctx);

    cout << "Session closed.\n";

    return 0;
}
