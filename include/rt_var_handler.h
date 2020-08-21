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
# RT variables handler header
#
###############################################################################*/

#ifndef RT_VAR_HANDLER_H
#define RT_VAR_HANDLER_H

#include "Common.h"
#include <sys/mman.h>
#include <fcntl.h>
#include "CrioLinux.h"

#ifdef __cplusplus
extern "C" {
#endif



bool is_rt_var(std::string name);
bool get_rt_var_sign(std::string name);
type_code get_rt_var_size(std::string name);
int open_shared_memory(std::string shm_name, uint8_t **memory_ptr, uint32_t shared_memory_size);
uint8_t decode_enum_size(enum type_code code);
int get_rt_val(uint8_t *shared_memory, uint64_t offset, double &value, std::string name);
int set_rt_val(uint8_t * shared_memory, uint64_t offset, double value, std::string name);
int get_rt_arr(uint8_t *shared_memory, uint64_t offset, void * arr, uint32_t size_bytes, enum type_code type, uint32_t size_elements);
int get_rt_str(uint8_t * shared_memory, uint64_t offset, char str[40]);
int set_rt_str(uint8_t * shared_memory, uint64_t offset, char str[40]);
#ifdef __cplusplus
}
#endif



#endif
