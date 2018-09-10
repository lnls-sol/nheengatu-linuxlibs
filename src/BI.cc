#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>

#include "CrioLinux.h"

#define CACHE_TIMEOUT_US 1000
#define DEBUG 1

/* boost namespace */
using boost::bimap;

/* typedefs */
typedef bimap< unsigned, std::string > bm_type;
typedef bimap< std::string, unsigned > bm_address_type;

/* prototypes */
static bool getBI(struct crio_context *ctx, uint32_t Index, uint64_t address);


int CrioReadBIArray(struct crio_context *ctx, uint64_t *Output, uint64_t address) {

    pthread_mutex_lock(&ctx->bi_mutex);
    clock_t delta = clock() - ctx->bi_sample_time;
    if (delta*(1000000.0/CLOCKS_PER_SEC) < CACHE_TIMEOUT_US && ctx->bi_cache_valid == true)
    {
        *Output = ctx->bi_cache;
        //printf("Using cached  ");
    } 
    else 
    {
        auto Res = NiFpga_ReadU64(NiFpga_Session(ctx->session), address, Output);
        if (NiFpga_IsError(Res)) return -1;
        ctx->bi_cache = *Output;
        ctx->bi_cache_valid = true;
        ctx->bi_sample_time = clock();
        //printf("Fetching data ");
    }
    pthread_mutex_unlock(&ctx->bi_mutex);
    return 0;
}



static bool getBI(struct crio_context *ctx, uint32_t index, uint64_t address){
    uint64_t output;
    CrioReadBIArray(ctx, &output, address);
    return (bool) (output & (0x1UL << index));
}

int CrioGetBIArrayItemByIndex(struct crio_context *ctx, bool *Item, uint32_t Index) {
    try {
        ((bm_type *)ctx->bi_map)->left.at(Index).c_str();
        *Item = getBI(ctx, Index, ((bm_address_type *)ctx->bi_addresses)->left.at("BI0") );
        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}

int CrioGetBIArraySize(struct crio_context *ctx, unsigned *size) {
    bm_type * x = (bm_type *)ctx->bi_map;
    *size = (*x).size();
    return 0;
}

int CrioGetBIArrayItemByName(struct crio_context *ctx, bool *Item, const char *Name) {
    uint64_t Index;

    try {
        Index = ((bm_type *)ctx->bi_map)->right.at(Name);
        *Item = getBI(ctx, Index, ((bm_address_type *)ctx->bi_addresses)->left.at("BI0"));
        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}


int CrioGetBIArrayItemName(struct crio_context *ctx, unsigned Item, const char **Name) {

    try {
        *Name = ((bm_type *)ctx->bi_map)->left.at(Item).c_str();
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

int CrioGetBIArrayItemNumber(struct crio_context *ctx, const char *Text, unsigned *Number) {

    unsigned Val;

    if (ParseNumberStrict(Text, &Val) == 0) {
        if (Val >= 32) return -1;

        *Number = Val;

        return 0;
    }

    try {
        *Number = ((bm_type *)ctx->bi_map)->right.at(Text);

        return 0;
    }
    catch (out_of_range) {
        return -1;
    }
}
