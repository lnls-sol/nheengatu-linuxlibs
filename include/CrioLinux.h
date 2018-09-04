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
struct crio_ctx {
    CrioSession      session;
    bool             session_open;
    clock_t          bi_sample_time;
    uint64_t         bi_cache;
    bool             bi_cache_valid;
    pthread_mutex_t  bi_mutex;
};


/* Functions - to be depricated */
int CrioReadBIArray(CrioSession Session, uint64_t *Output);
int CrioGetBIArrayItemName(CrioSession Session, unsigned Item, const char **Name);
int CrioGetBIArrayItemNumber(CrioSession Session, const char *Text, unsigned *Number);

/* Functions */
int  CrioSetup(CrioSession *Session);
void CrioCleanup(CrioSession Session);
int  CrioGetBIArrayItemByName(CrioSession Session, const char *Name, bool *Item); 
int  CrioGetBIArrayItemByIndex(CrioSession Session, bool *Item, uint32_t Index);

#ifdef __cplusplus
}
#endif

#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif

#endif /* CRIOLINUX_H */
