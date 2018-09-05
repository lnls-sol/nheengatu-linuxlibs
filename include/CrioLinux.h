#ifndef CRIOLINUX_H
#define CRIOLINUX_H

#include <stdint.h>

#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CRIO_BI_ARRAY_COUNT 32

/* Type definitions */
typedef	unsigned int CrioSession;

/* Structures */
struct crio_context {
    CrioSession      session;
    bool             session_open;
    clock_t          bi_sample_time;
    uint64_t         bi_cache;
    bool             bi_cache_valid;
    pthread_mutex_t  bi_mutex;
    void           * bi_map;
};


/* Available Functions */
int  CrioSetup(struct crio_context*);
void CrioCleanup(struct crio_context*);
int CrioReadBIArray(struct crio_context*, uint64_t *output);
int  CrioGetBIArrayItemByName(struct crio_context*, const char *name, bool *item);
int  CrioGetBIArrayItemByIndex(struct crio_context*, bool *item, uint32_t index);


/* Functions - to be depricated */
int CrioGetBIArrayItemName(CrioSession Session, unsigned Item, const char **Name);
int CrioGetBIArrayItemNumber(CrioSession Session, const char *Text, unsigned *Number);
#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
