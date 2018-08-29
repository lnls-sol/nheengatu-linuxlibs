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
    {0, "Mod3/DIO0"},
    {1, "Mod3/DIO1"},
    {2, "Mod3/DIO2"},
    {3, "Mod3/DIO3"},
    {4, "Mod3/DIO4"},
    {5, "Mod3/DIO5"},
    {6, "Mod3/DIO6"},
    {7, "Mod3/DIO7"},
    {8, "Mod3/DIO8"},
    {9, "Mod3/DIO9"},
    {10, "Mod3/DI10"},
    {11, "Mod3/DI11"},
    {12, "Mod3/DI12"},
    {13, "Mod3/DI13"},
    {14, "Mod3/DI14"},
    {15, "Mod3/DI15"},
    {16, "Mod3/DI16"},
    {17, "Mod3/DI17"},
    {18, "Mod3/DI18"},
    {19, "Mod3/DI19"},
    {20, "Mod3/DI20"},
    {21, "Mod3/DI21"},
    {22, "Mod3/DI22"},
    {23, "Mod3/DI23"},
    {24, "Mod3/DI24"},
    {25, "Mod3/DI25"},
    {26, "Mod3/DI26"},
    {27, "Mod3/DI27"},
    {28, "Mod3/DI28"},
    {29, "Mod3/DI29"},
    {30, "Mod3/DI30"},
    {31, "USER Push Button"},
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
