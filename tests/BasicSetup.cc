#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>

int main(void) {
    CrioSession Session;
    Res = CrioSetup(&Session);
    assert(Res == 0);

    cout << "Session opened.\n";

    CrioCleanup(Session);

    cout << "Session closed.\n";

    return 0;
}
