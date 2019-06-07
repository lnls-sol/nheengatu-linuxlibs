#ifndef ANALOG_H
#define ANALOG_H

#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "CrioLinux.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FXP_SUPPORTED_COUNT 256

using namespace std;


struct fxp_ctx{
    bool           sign;
    uint32_t       word_length;
    uint32_t       int_word_length;
    uint32_t       address;
};




#ifdef __cplusplus
}
#endif

#endif /* ANALOG_H */
