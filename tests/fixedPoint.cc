#undef NDEBUG
#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
 #include<unistd.h>
#include<string.h>
#include <ctime>    // For time()
#include <cstdlib>

#include <math.h>
using namespace std;

#define U64LEN 64UL

double fxp_to_dbl(uint64_t value, struct fxp_ctx * fxp_data);
uint64_t dbl_to_fxp(double value, struct fxp_ctx * fxp_data);

struct fxp_ctx{
    bool           sign;
    uint32_t       word_length;
    uint32_t       int_word_length;
    uint32_t       address;
};


uint64_t dbl_to_fxp(double value, struct fxp_ctx * fxp_data)
{
    double sign_masked = value;
    uint64_t fxp = 0;
    if (fxp_data->sign)
        sign_masked = abs(value);
    uint64_t decimal_word_length = fxp_data->word_length - fxp_data->int_word_length;

    /* shift left for 1UL << 64 fails so work arround was implemented */
    if (decimal_word_length == U64LEN)
        fxp = (uint64_t)(round(sign_masked * 18446744073709551616.0));
    else
        fxp = (uint64_t)(round(sign_masked * (1ULL << decimal_word_length)));
    if (fxp_data->sign && value < 0)
    {

        /* calculate 2's complement of the integer part */
        fxp = ~fxp;
        fxp++;

        /* Clean upper bits */
        if (U64LEN-fxp_data->word_length != U64LEN)
        {
            fxp = fxp << (U64LEN-fxp_data->word_length);
            fxp = fxp >> (U64LEN-fxp_data->word_length);
        }
    }
    return fxp;

}

double fxp_to_dbl(uint64_t value, struct fxp_ctx * fxp_data)
{
    uint64_t twos_comp = value;
    double value_double = 0;
    uint64_t sign_bit = value & (1ULL << (fxp_data->word_length - 1));
    uint64_t decimal_word_length = fxp_data->word_length - fxp_data->int_word_length;
    if (fxp_data->sign && sign_bit != 0)
    {
        /* calculate 2's complement of the integer part */
        twos_comp = ~twos_comp;
        twos_comp++;

        /* Clean upper bits */
        if (U64LEN-fxp_data->word_length != U64LEN)
        {
            twos_comp = twos_comp << (U64LEN-fxp_data->word_length);
            twos_comp = twos_comp >> (U64LEN-fxp_data->word_length);
        }
    }
    /* shift left for 1UL << 64 fails so work arround was implemented */
    if (decimal_word_length == U64LEN)
        value_double = ((double)twos_comp / 18446744073709551616.0);
    else
        value_double = ((double)twos_comp / (double)(1ULL << decimal_word_length));
    if (fxp_data->sign && sign_bit != 0)
        value_double = 0-value_double;
    return value_double;
}


int main()
{
    uint64_t fxp = 8307674973655721984ULL;
    uint64_t fxp_ret = 0;
    struct fxp_ctx fxp_data = {
        .sign = true,
        .word_length  = 64,
        .int_word_length = 0,
        .address = 1234
    };
    cout << fxp << endl;
    double ret = fxp_to_dbl(fxp, &fxp_data);
    cout.precision(64);
    cout << ret << endl;
    fxp_ret = dbl_to_fxp(ret, &fxp_data);
    cout << fxp_ret << endl;
    return 0;

}
