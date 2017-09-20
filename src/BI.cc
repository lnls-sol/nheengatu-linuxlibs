#include "Common.h"
#include "CrioLinux.h"

int CrioReadBIArray(CrioSession Session, uint64_t *Output) {
    auto Res = NiFpga_ReadU64(NiFpga_Session(Session),
            NiFpga_CrioLinux_IndicatorU64_BIArray, Output);
    if (NiFpga_IsError(Res)) return -1;

    return 0;
}

int CrioGetBIArrayItemName(CrioSession Session, unsigned Item, const char **Name) {
	static const char *Names[] = {
		"USER Push Button",
		"Scan Clock",
		"Mod2/DIO0",
		"Mod2/DIO1",
		"Mod2/DIO2",
		"Mod2/DIO3",
		"Mod3/DIO0",
		"Mod3/DIO1",
		"Mod3/DIO2",
		"Mod3/DIO3",
		"Mod3/DIO4",
		"Mod3/DIO5",
		"Mod3/DIO6",
		"Mod3/DIO7",
		"Mod4/DIO0",
		"Mod4/DIO1",
		"Mod4/DIO2",
		"Mod4/DIO3",
		"Mod4/DIO4",
		"Mod4/DIO5",
		"Mod4/DIO6",
		"Mod4/DIO7",
		"Mod5/DIO0",
		"Mod5/DIO1",
		"Mod5/DIO2",
		"Mod5/DIO3",
		"Mod5/DIO4",
		"Mod5/DIO5",
		"Mod5/DIO6",
		"Mod5/DIO7",
	};
	static const unsigned NameCount = sizeof(Names)/sizeof(Names[0]);
	static_assert(NameCount == CRIO_BI_ARRAY_COUNT, "Name array does not match array count");

	if (Item >= NameCount) return -1;

	*Name = Names[Item];

	return 0;
}
