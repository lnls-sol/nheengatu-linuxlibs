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
# SCALER Header containing context definition
#
###############################################################################*/

#ifndef SCALER_H
#define SCALER_H

#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
 #include<unistd.h>
#include <NiFpga.h>
#include<string.h>

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

#define MAX_SCALER_CHANNELS 64
#define MAX_SCALER_SUPPORTED_COUNT 10
struct scaler_ctx{
    uint32_t   reset_addr;
    uint32_t   enable_addr;
    uint32_t   gate_array_addr;
    uint32_t   oneshot_addr;
    uint32_t   counter_array_addr;
    uint32_t   pr_array_addr;
    uint32_t   num_of_counters;
    uint32_t   done_addr;
    uint32_t   scaler_preset_cache[MAX_SCALER_CHANNELS];
    bool       scaler_gate_cache[MAX_SCALER_CHANNELS];
};

#ifdef __cplusplus
}
#endif

#endif /* SCALER_H */
