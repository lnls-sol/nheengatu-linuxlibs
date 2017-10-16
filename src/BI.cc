#include "Common.h"

#include <ctype.h>
#include <errno.h>
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

static int ParseNumberStrict(const char *Text, unsigned *Value) {
    char *End;
    long long Val;

    /* Blank string returns error */
    while (*Text != '\0' && isspace(*Text)) Text++;
    if (*Text == '\0') return -1;

    /* Use signed parsing to avoid implicit modular conversion */
    errno = 0;
    Val = strtoll(Text, &End, 0);
    if (errno != 0) return -1;

    /* Trailing text returns error */
    while (*End != '\0' && isspace(*End)) End++;
    if (*End != '\0') return -1;

    /* Handle unsigned range errors */
    if (Val < 0) return -1;
    if (Val > (long long)UINT_MAX) return -1;

    *Value = Val;

    return 0;
}

int CrioGetBIArrayItemNumber(CrioSession Session, const char *Text, unsigned *Number) {
    assert(NAME_BIMAP.size() == CRIO_BI_ARRAY_COUNT);

    unsigned Val;

    if (ParseNumberStrict(Text, &Val) == 0) {
        if (Val >= CRIO_BI_ARRAY_COUNT) return -1;

        *Number = Val;

        return 0;
    }

    try {
        *Number = NAME_BIMAP.right.at(Text);

        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}
