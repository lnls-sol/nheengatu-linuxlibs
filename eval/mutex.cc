#undef NDEBUG
#include "shared_mutex.h"
#include <stdio.h>
#include "Common.h"
#include <CrioLinux.h>
#include <time.h>

int main() {
    // Init shared mutex by a name, which can be used by
    // any other process to access the mutex.
    // This function both creates new and opens an existing mutex.
    struct crio_context ctx;
    clock_t start, stop, diff;
    uint64_t Output;
    int ret;
    uint64_t reads=10000;
    auto Res = CrioSetup(&ctx);
    assert(Res == 0);
    pthread_mutex_t mutex_dummy;

    pthread_mutex_init(&mutex_dummy, NULL);
    char mstr[] = "/my-mutex";
    shared_mutex_t mutex = shared_mutex_init(mstr);
    if (mutex.ptr == NULL) {
        return 1;
    }

    if (mutex.created) {
        printf("The mutex was just created\n");
    }

    //Read X time and calculate time
    start = clock();
    for (uint64_t i = 0; i < reads; i++)
    {
        ret = pthread_mutex_lock(mutex.ptr);
        if (ret == EOWNERDEAD)
        {
            pthread_mutex_consistent(mutex.ptr);
            printf("Mutex status inconsistent. Correcting\n" );
        }
        if (ret == ENOTRECOVERABLE)
        {
            printf("Mutex status not recoverable detected. Deleting shared memory.\n" );
            shared_mutex_destroy(mutex);
            return -1;
        }
        Res = CrioReadBIArray(&ctx, &Output, 0x18000);
        ret = pthread_mutex_unlock(mutex.ptr);
    }
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu FPGA with shared mutex (%f read+shared mutex/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));



    start = clock();
    for (uint64_t i = 0; i < reads; i++)
    {
        ret = pthread_mutex_lock(mutex.ptr);
        if (ret == EOWNERDEAD)
        {
            pthread_mutex_consistent(mutex.ptr);
            printf("Mutex status inconsistent. Correcting\n" );
        }
        Res = Output;
        pthread_mutex_unlock(mutex.ptr);
    }
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu assignments (cache) (%f shared mutex locks/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));


    start = clock();
    for (uint64_t i = 0; i < reads; i++)
    {
        pthread_mutex_lock(&mutex_dummy);
        Res = Output;
        pthread_mutex_unlock(&mutex_dummy);
    }
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu assignments (%f dummy mutex locks/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));

  
    start = clock();
    for (uint64_t i = 0; i < reads; i++)
    {
        Res = stop-start;
    }
    stop = clock();
    diff = stop - start;
    printf ("It took %ld ticks (%f seconds) for %lu subtractions (time passed evaluation) (%f subtractions/ms).\n",diff,((float)diff)/CLOCKS_PER_SEC, reads, reads/(((float)diff * 1000)/CLOCKS_PER_SEC));

    
    ret = pthread_mutex_lock(mutex.ptr);
    if (ret == EOWNERDEAD)
    {
        pthread_mutex_consistent(mutex.ptr);
        printf("Mutex status inconsistent. Correcting\n" );
    }
    printf("Press any key to unlock the mutex");
    getchar();
    pthread_mutex_unlock(mutex.ptr);

    // Closing is used to release local resources, used by a mutex.
    // It's still available to any other process.
    if (shared_mutex_close(mutex)) {
        return 1;
    }
    return 0;
}
