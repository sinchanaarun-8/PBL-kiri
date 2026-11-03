#ifndef CORE_SHARED_MEMORY_H
#define CORE_SHARED_MEMORY_H

#include <pthread.h>
#include "core.h"

#define CORE_SHM_NAME "/pbl_core_shared_memory"
#define CORE_SHM_MAGIC 0x50424C32u
#define CORE_SHM_VERSION 1u

typedef struct
{
    unsigned int magic;
    unsigned int version;
    unsigned long update_count;
    int ready;
    pthread_mutex_t mutex; /* process-shared synchronization */
    CoreState state;       /* live simulator state */
} SharedCoreMemory;

typedef struct
{
    int fd;
    SharedCoreMemory *ptr;
} CoreShmHandle;

int core_shm_create(CoreShmHandle *handle);
int core_shm_attach(CoreShmHandle *handle);
void core_shm_lock(SharedCoreMemory *shared);
void core_shm_unlock(SharedCoreMemory *shared);
void core_shm_close(CoreShmHandle *handle);
void core_shm_unlink(void);

#endif
