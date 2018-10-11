#ifndef UTILS_H
#define UTILS_H

#include "Common.h"
#include <boost/bimap.hpp>

typedef boost::bimap< std::string, unsigned > bm_address_type;

#ifdef __cplusplus
extern "C" {
#endif

void print_bmap(bm_address_type * bmap);

#ifdef __cplusplus
}
#endif

#endif
