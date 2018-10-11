#ifndef RT_VAR_HANDLER_H
#define RT_VAR_HANDLER_H

#include "Common.h"
#include <sys/mman.h>
#include <fcntl.h>

#ifdef __cplusplus
extern "C" {
#endif



enum type_code{
    DBL,
    SGL,
    U64,
    U32,
    U16,
    U08,
    I64,
    I32,
    I16,
    I08,
    BOL,
    UNKNOWN
};


bool is_rt_var(std::string name);
bool get_rt_var_sign(std::string name);
type_code get_rt_var_size(std::string name);
int open_shared_memory(std::string shm_name, uint8_t **memory_ptr);
uint8_t decode_enum_size(enum type_code code);

#ifdef __cplusplus
}
#endif

#endif
