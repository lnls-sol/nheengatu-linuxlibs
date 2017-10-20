#include "Common.h"
#include <stdexcept>

#include <boost/bimap.hpp>

#include "CrioLinux.h"

using boost::bimap;

typedef bimap<unsigned, string> NameBimap;

int CrioReadBIArray(CrioSession Session, uint64_t *Output) {
    auto Res = NiFpga_ReadU64(NiFpga_Session(Session),
            NiFpga_CrioLinux_IndicatorU64_BIArray, Output);
    if (NiFpga_IsError(Res)) return -1;

    return 0;
}

static NameBimap MakeNameBimap(initializer_list<NameBimap::value_type> List) {
    return NameBimap(begin(List), end(List));
}

static const NameBimap NAME_BIMAP = MakeNameBimap({
    {0, "USER Push Button"},
    {1, "Scan Clock"},
    {2, "Mod2/DIO0"},
    {3, "Mod2/DIO1"},
    {4, "Mod2/DIO2"},
    {5, "Mod2/DIO3"},
    {6, "Mod3/DIO0"},
    {7, "Mod3/DIO1"},
    {8, "Mod3/DIO2"},
    {9, "Mod3/DIO3"},
    {10, "Mod3/DIO4"},
    {11, "Mod3/DIO5"},
    {12, "Mod3/DIO6"},
    {13, "Mod3/DIO7"},
    {14, "Mod4/DIO0"},
    {15, "Mod4/DIO1"},
    {16, "Mod4/DIO2"},
    {17, "Mod4/DIO3"},
    {18, "Mod4/DIO4"},
    {19, "Mod4/DIO5"},
    {20, "Mod4/DIO6"},
    {21, "Mod4/DIO7"},
    {22, "Mod5/DIO0"},
    {23, "Mod5/DIO1"},
    {24, "Mod5/DIO2"},
    {25, "Mod5/DIO3"},
    {26, "Mod5/DIO4"},
    {27, "Mod5/DIO5"},
    {28, "Mod5/DIO6"},
    {29, "Mod5/DIO7"},
});

int CrioGetBIArrayItemName(CrioSession Session, unsigned Item, const char **Name) {
    assert(NAME_BIMAP.size() == CRIO_BI_ARRAY_COUNT);

    try {
        *Name = NAME_BIMAP.left.at(Item).c_str();
        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}
