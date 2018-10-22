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
