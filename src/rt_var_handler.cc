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
# rt variable handling
#
###############################################################################*/

#include "rt_var_handler.h"




std::string SHM_PATH = "/dev/shm";

bool is_rt_var(std::string name){
    if (name.compare(0,3,"RT_") == 0)
        return true;
    return false;
}


uint8_t decode_enum_size(enum type_code code){

    /* If sanitizer is enabled, this function must return 8 for any size.
     * Due to the sanitizer library that is used when compiling with docker
     * each type has some specific alignment in the memory. For simplicity
     * we aligned all variables with addresses of multiples of 8.
    */
    switch (code)
    {
        case DBL: return 8;
        case SGL: return 4;
        case U64: return 8;
        case U32: return 4;
        case U16: return 2;
        case U08: return 1;
        case I64: return 8;
        case I32: return 4;
        case I16: return 2;
        case I08: return 1;
        case BOL: return 1;
        case STO: return 40;
        case STI: return 40;
    default:throw CrioLibException(E_NOT_FOUND, "Unrecognized size <%x>", (uint8_t)code);
    }
}



int set_rt_str(uint8_t * shared_memory, uint64_t offset, char str[40])
{
    memcpy((char *)(shared_memory + offset), str, 40);
    return 0;
}

int get_rt_str(uint8_t * shared_memory, uint64_t offset, char str[40])
{
    memcpy(str, (char *)(shared_memory + offset), 40);
    return 0;
}

int set_rt_val(uint8_t * shared_memory, uint64_t offset, double value, std::string name)
{
    double *dbl;
    float *flt;
    uint8_t *u08;
    uint16_t *u16;
    uint32_t *u32;
    uint64_t *u64;
    int8_t  *i08;
    int16_t *i16;
    int32_t *i32;
    int64_t *i64;
    switch (get_rt_var_size(name))
    {
        case DBL: dbl = (double*)(shared_memory + offset);
                  *dbl = value;
                  break;
        case SGL: flt = (float*)(shared_memory + offset);
                  *flt = value;
                  break;
        case U64: u64 = (uint64_t*)(shared_memory + offset);
                  *u64 = value;
                  break;
        case U32: u32 = (uint32_t*)(shared_memory + offset);
                  *u32 = value;
                  break;
        case U16: u16 = (uint16_t*)(shared_memory + offset);
                  *u16 = value;
                  break;
        case U08: u08 = (uint8_t*)(shared_memory + offset);
                  *u08 = value;
                  break;
        case I64: i64 = (int64_t*)(shared_memory + offset);
                  *i64 = value;
                  break;
        case I32: i32 = (int32_t*)(shared_memory + offset);
                  *i32 = value;
                  break;
        case I16: i16 = (int16_t*)(shared_memory + offset);
                  *i16 = value;
                  break;
        case I08:
        case BOL: i08 = (int8_t*)(shared_memory + offset);
                  *i08 = value;
                  break;
        default: break;
    }
    return 0;
}



int get_rt_arr(uint8_t *shared_memory, uint64_t offset, void * arr, uint32_t size_bytes, enum type_code type, uint32_t size_elements)
{
    if (type == U64)
    {
        uint64_t * arr_src = (uint64_t *)(shared_memory+offset);
        double * arr_dst = (double *) arr;
        for (uint32_t  i =0; i < size_elements; i++)
            arr_dst[i] = (double)arr_src[i];
    } else if (type == I64)
    {
        int64_t * arr_src = (int64_t *)(shared_memory+offset);
        double * arr_dst = (double *) arr;
        for (uint32_t  i =0; i < size_elements; i++)
            arr_dst[i] = (double)arr_src[i];
    }
    else
    {
        memcpy(arr, shared_memory+offset, size_bytes);
    }
    return 0;
}


int get_rt_val(uint8_t *shared_memory, uint64_t offset, double &value, std::string name)
{
    double dbl;
    float flt;
    uint8_t u08;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    int8_t  i08;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    bool boolean;
    switch (get_rt_var_size(name))
    {
        case DBL: dbl = *(double*)(shared_memory + offset);
                  value = static_cast<double>(dbl);
                  break;
        case SGL: flt = *(float*)(shared_memory + offset);
                  value = static_cast<double>(flt);
                  break;
        case U64: u64 = *(uint64_t*)(shared_memory + offset);
                  // This conversion is lossy. Double precision is less than u64.
                  value = static_cast<double>(u64);
                  break;
        case U32: u32 = *(uint32_t*)(shared_memory + offset);
                  value = static_cast<double>(u32);
                  break;
        case U16: u16 = *(uint16_t*)(shared_memory + offset);
                  value = static_cast<double>(u16);
                  break;
        case U08: u08 = *(uint8_t*)(shared_memory + offset);
                  value = static_cast<double>(u08);
                  break;
        case I64: i64 = *(int64_t*)(shared_memory + offset);
                  // This conversion is lossy. Double precision is less than i64.
                  value = static_cast<double>(i64);
                  break;
        case I32: i32 = *(int32_t*)(shared_memory + offset);
                  value = static_cast<double>(i32);
                  break;
        case I16: i16 = *(int16_t*)(shared_memory + offset);
                  value = static_cast<double>(i16);
                  break;
        case I08: i08 = *(int8_t*)(shared_memory + offset);
                  value = static_cast<double>(i08);
                  break;
        case BOL: boolean = *(int8_t*)(shared_memory + offset);
              value = static_cast<double>(boolean);
              break;
        default: value = 0; break;
    }
    return 0;
}


enum type_code get_rt_var_size(std::string name){

    if (name.compare(3,3,"DBL") == 0) return DBL;
    if (name.compare(3,3,"SGL") == 0) return SGL;
    if (name.compare(3,3,"U64") == 0) return U64;
    if (name.compare(3,3,"U32") == 0) return U32;
    if (name.compare(3,3,"U16") == 0) return U16;
    if (name.compare(3,3,"U08") == 0) return U08;
    if (name.compare(3,3,"I64") == 0) return I64;
    if (name.compare(3,3,"I32") == 0) return I32;
    if (name.compare(3,3,"I16") == 0) return I16;
    if (name.compare(3,3,"I08") == 0) return I08;
    if (name.compare(3,3,"BOL") == 0) return BOL;
    if (name.compare(3,3,"MBI") == 0) return U16;
    if (name.compare(3,3,"MBO") == 0) return U16;
    if (name.compare(3,3,"STO") == 0) return STO;
    if (name.compare(3,3,"STI") == 0) return STI;

    return UNKNOWN;
}

bool get_rt_var_sign(std::string name){
    if (name.compare(3,1,"U") == 0)
        return false;
    else
        return true;
}

int open_shared_memory(std::string shm_name, uint8_t **memory_ptr, uint32_t shared_memory_size)
{
    if (access((SHM_PATH+shm_name).c_str(), F_OK) != 0) throw (CrioLibException(E_SHARED_MEM, "[%s] Cannot find shared memory file <%s>.", LIB_CRIO_LINUX, shm_name.c_str()));

    // Open file descriptor to shared memory.
    int fd = shm_open(shm_name.c_str(), O_RDWR, (mode_t) 0777);
    if (fd < 0) throw (CrioLibException(E_SHARED_MEM, "[%s] Cannot open shared memory file <%s>.", LIB_CRIO_LINUX, shm_name.c_str()));

    // Map memory from shared memory to application memory.
    if ((shared_memory_size % 4096) != 0)
        throw (CrioLibException(E_SHARED_MEM, "Shared memory size is not a multiple of 4096 <%u>.", shared_memory_size));
    void *mmapPointer = mmap(NULL, shared_memory_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mmapPointer == MAP_FAILED) throw (CrioLibException(E_SHARED_MEM, "[%s] Cannot map shared memory file.", LIB_CRIO_LINUX));

    // Since file descriptor is no longer needed after mmap, close it.
    close(fd);

    *memory_ptr = (uint8_t *)mmapPointer;
    return 0;
}
