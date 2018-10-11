#include "utils.h"


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
