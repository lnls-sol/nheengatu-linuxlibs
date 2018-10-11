#ifndef RT_VAR_HANDLER_H
#define RT_VAR_HANDLER_H

#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif


enum type_code{
    DBL = 8,
    SGL = 4,
    U64 = 8,
    U32 = 4,
    U16 = 2,
    U08 = 1,
    I64 = 4,
    I32 = 4,
    I16 = 2,
    I08 = 1
};


bool is_rt_var(std::string name);
bool get_rt_var_sign(std::string name);
uint8_t get_rt_var_size(std::string name);

#ifdef __cplusplus
}
#endif

#endif
