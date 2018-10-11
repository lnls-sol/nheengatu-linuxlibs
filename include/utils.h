#ifndef UTILS_H
#define UTILS_H

#include "Common.h"
#include <boost/bimap.hpp>

typedef boost::bimap< std::string, unsigned > bm_address_type;

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
void print_bmap(bm_address_type * bmap);

#ifdef __cplusplus
}
#endif

#endif
