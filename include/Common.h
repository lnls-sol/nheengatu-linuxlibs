#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
 #include<unistd.h>
#include <NiFpga.h>
#include<string.h>
using namespace std;

#define CFG_FILE "/usr/local/epics/config/cfg.ini"


struct scaler_ctx{
    uint32_t   reset_addr;
    uint32_t   enable_addr;
    uint32_t   gate_array_addr;
    uint32_t   oneshot_addr;
    uint32_t   counter_array_addr;
    uint32_t   pr_array_addr;
    uint32_t   num_of_counters;
    uint32_t   done_addr;
    uint32_t * scaler_preset_cache;
    bool     * scaler_gate_cache;
};

#endif /* COMMON_H */
