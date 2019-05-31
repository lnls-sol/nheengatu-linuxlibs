#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "Common.h"

#include <ctype.h>
#include <errno.h>
#include <stdexcept>
#include <time.h>
#include <boost/bimap.hpp>
#include "rt_var_handler.h"
#include "CrioLinux.h"


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_WAVEFORM_SUPPORTED_COUNT 256

using namespace std;


enum type_code get_wf_size(std::string name);

struct waveform_ctx{
    enum type_code waveform_type;
    uint32_t       waveform_size_elements;
    uint32_t       waveform_addr;
    uint32_t       waveform_type_bytes;
    uint64_t       waveform_size_bytes;
};

bool is_waveform(void * waveform_name_index_map, const char *name);
int populate_rt_offset_arr(void *waveform_name_index_map, uint8_t * rt_variable_offsets, int rt_var_size, void *rt_addresses, struct waveform_ctx *waveforms);


#ifdef __cplusplus
}
#endif

#endif /* WAVEFORM_H */
