#include "utils.h"



void debug(struct crio_context * ctx, int enable)
{
    if (enable == 1)
    {
        ctx->log = fopen("/opt/autosave/debug_log.txt", "w");
        if (ctx->log == NULL)
        {
            throw (CrioLibException(E_RESOURCE_ALLOC , "[%s] Could not open log file for writing\n", LIB_CRIO_LINUX  ));
            return;
        }
        ctx->debugCRIO = true;
        printf("CRIO debug enabled\n");
    }
    else
    {
        ctx->debugCRIO = false;
        if (ctx->log != NULL)
        {
            fclose(ctx->log);
            ctx->log = NULL;
        }
        printf("CRIO debug disabled\n");
    }
}

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

bool is_fixed_point(std::string name){
    if (name.compare(0,3,"FXP") == 0)
        return true;
    return false;
}

void print_bmap(bm_address_type * bmap)
{
    for( bm_address_type::const_iterator iter = bmap->begin(), iend = bmap->end(); iter != iend; ++iter )
        std::cout << iter->left << " <--> " << iter->right << std::endl;
}

void set_cpu(uint cpu)
{
    //cpu_set_t cpu;
    //CPU_ZERO(&cpu);
    //CPU_SET(cpu, &cpu);
    //printf("Setting CPU Affinity to CPU%d... ", 1);
    //ret = sched_setaffinity(0, sizeof(cpu_set_t), &cpu);
    //printWithStatus("", ret);

    // Elevate this process to highest priority.
    //struct sched_param params;
    //params.sched_priority = 99;
    //printf("Setting thread priority to %d... ", 99);
    //ret = pthread_setschedparam(pthread_self(), SCHED_FIFO, &params);
    //printWithStatus("", ret);
}


void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
    return;
}

void printLibVersion(void)
{
    printVersion();
}
