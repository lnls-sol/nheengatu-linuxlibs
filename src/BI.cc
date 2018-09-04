#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>

#include "CrioLinux.h"

#define CACHE_TIMEOUT_US 500
#define DEBUG 1

using boost::bimap;
extern struct crio_ctx ctx;
static bool getBI(CrioSession Session, uint32_t Index);
typedef bimap<unsigned, string> NameBimap;

int CrioReadBIArray(CrioSession Session, uint64_t *Output) {

    pthread_mutex_lock(&ctx.bi_mutex);
    clock_t delta = clock() - ctx.bi_sample_time;
    if (delta*(1000000.0/CLOCKS_PER_SEC) < CACHE_TIMEOUT_US && ctx.bi_cache_valid == true)
    {
        *Output = ctx.bi_cache;
        printf("Using cached  ");         
    } 
    else 
    {
        auto Res = NiFpga_ReadU64(NiFpga_Session(Session),
            NiFpga_CrioLinux_IndicatorU64_BIArray, Output);
        if (NiFpga_IsError(Res)) return -1;
        ctx.bi_cache = *Output;
        ctx.bi_cache_valid = true;
        ctx.bi_sample_time = clock();
        printf("Fetching data ");         
    }
    pthread_mutex_unlock(&ctx.bi_mutex); 
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

static bool getBI(CrioSession Session, uint32_t Index){
    uint64_t Output;
    CrioReadBIArray(Session, &Output); 
    return (bool) (Output & (0x1UL << Index));
}

int CrioGetBIArrayItemByIndex(CrioSession Session, bool *Item, uint32_t Index) {
    assert(NAME_BIMAP.size() == CRIO_BI_ARRAY_COUNT);
    try {
        NAME_BIMAP.left.at(Index).c_str();
        *Item = getBI(Session, Index);
        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}

int CrioGetBIArrayItemByName(CrioSession Session, const char *Name, bool *Item) {
    assert(NAME_BIMAP.size() == CRIO_BI_ARRAY_COUNT);
    uint64_t Index;

    try {
        Index = NAME_BIMAP.right.at(Name);
        *Item = getBI(Session, Index);
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
