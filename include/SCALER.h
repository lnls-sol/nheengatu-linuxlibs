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
