#undef NDEBUG
#include "Common.h"
#include <CrioLinux.h>

int main(void) {

    struct crio_context ctx;
    auto Res = CrioSetup(&ctx);
    assert(Res == 0);

    uint64_t Output;
    Res = CrioReadBIArray(&ctx, &Output);
    assert(Res == 0);

    cout << "Binary input values:\n";
    for (unsigned I = 0; I < CRIO_BI_ARRAY_COUNT; I++) {
	    const char *Name;
        Res = CrioGetBIArrayItemName(ctx.session, I, &Name);
	    assert(Res == 0);

	    cout << Name << ": " << (bool)(Output & (1<<I)) << '\n';
    }
    cout << '\n';

    CrioCleanup(&ctx);
    cout << "FINISHED.\n";

    return 0;
}
