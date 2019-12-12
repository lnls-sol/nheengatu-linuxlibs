/*###############################################################################
#
# This software is distributed under the following ISC license:
#
# Copyright © 2017 BRAZILIAN SYNCHROTRON LIGHT SOURCE <sol@lnls.br>
#   Dawood Alnajjar <dawood.alnajjar@lnls.br>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
# OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
# CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
# Description:
# Utils.h header, contains several useful function that do not
# belong to other places.
#
###############################################################################*/

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
int throwCRIOError(int error);

#ifdef __cplusplus
}
#endif

#endif
