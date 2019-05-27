#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
 #include<unistd.h>
#include<string.h>
#include<CrioLinux.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MAX_WAVEFORM_SUPPORTED_COUNT 256

using namespace std;


enum type_code get_wf_size(std::string name);

struct waveform_ctx{
    enum type_code waveform_type;
    uint32_t       waveform_size;
    uint32_t       waveform_addr;
};

#ifdef __cplusplus
}
#endif

#endif /* WAVEFORM_H */
