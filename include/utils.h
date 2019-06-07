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
#include <CrioLinux.h>
#include "version.hh"
#include "ANALOG.h"
#include <math.h>

typedef boost::bimap< std::string, unsigned > bm_address_type;

#define U64LEN 64UL

#ifdef __cplusplus
extern "C" {
#endif

void print_bmap(bm_address_type * bmap);
void set_cpu(uint cpu);
void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result);
bool is_fixed_point(std::string name);
uint64_t dbl_to_fxp(double value, struct fxp_ctx * fxp_data);
double fxp_to_dbl(uint64_t value, struct fxp_ctx * fxp_data);

#ifdef __cplusplus
}
#endif

#endif
