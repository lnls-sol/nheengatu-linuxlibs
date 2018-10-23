#ifndef UTILS_H
#define UTILS_H

#include "Common.h"
#include <boost/bimap.hpp>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sched.h>
#include <time.h>



typedef boost::bimap< std::string, unsigned > bm_address_type;

#ifdef __cplusplus
extern "C" {
#endif

void print_bmap(bm_address_type * bmap);
void set_cpu(uint cpu);
void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result);

#ifdef __cplusplus
}
#endif

#endif
