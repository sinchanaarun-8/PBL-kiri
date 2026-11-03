#define _POSIX_C_SOURCE 200809L
#include "shared_memory.h"

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int core_shm_create(CoreShmHandle *handle)
{
    pthread_mutexattr_t attr;

    if (!handle) return 0;
    handle->fd = -1;
    handle->ptr = NULL;

    /* O_CREAT|O_EXCL prevents silently reusing an old shared-memory object. */
    handle->fd = shm_open(CORE_SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0660);
    if (handle->fd == -1) {
        if (errno == EEXIST) {
            shm_unlink(CORE_SHM_NAME); /* remove stale object from a prior crash */
            handle->fd = shm_open(CORE_SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0660);
        }
    }
    if (handle->fd == -1) {
        perror("shm_open create");
        return 0;
    }
    if (ftruncate(handle->fd, (off_t)sizeof(SharedCoreMemory)) == -1) {
        perror("ftruncate");
        close(handle->fd); handle->fd = -1;
        shm_unlink(CORE_SHM_NAME);
        return 0;
    }
    handle->ptr = mmap(NULL, sizeof(SharedCoreMemory),
                       PROT_READ | PROT_WRITE, MAP_SHARED, handle->fd, 0);
    if (handle->ptr == MAP_FAILED) {
        perror("mmap");
        handle->ptr = NULL; close(handle->fd); handle->fd = -1;
        shm_unlink(CORE_SHM_NAME);
        return 0;
    }

    memset(handle->ptr, 0, sizeof(*handle->ptr));
    if (pthread_mutexattr_init(&attr) != 0) return 0;
    if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        pthread_mutexattr_destroy(&attr);
        return 0;
    }
    if (pthread_mutex_init(&handle->ptr->mutex, &attr) != 0) {
        pthread_mutexattr_destroy(&attr);
        return 0;
    }
    pthread_mutexattr_destroy(&attr);

    handle->ptr->version = CORE_SHM_VERSION;
    handle->ptr->magic = CORE_SHM_MAGIC;
    handle->ptr->ready = 1;
    return 1;
}

int core_shm_attach(CoreShmHandle *handle)
{
    struct stat st;
    if (!handle) return 0;
    handle->fd = -1; handle->ptr = NULL;
    handle->fd = shm_open(CORE_SHM_NAME, O_RDWR, 0660);
    if (handle->fd == -1) { perror("shm_open attach"); return 0; }
    if (fstat(handle->fd, &st) == -1 ||
        st.st_size != (off_t)sizeof(SharedCoreMemory)) {
        fprintf(stderr, "Shared memory size/version mismatch.\\n");
        close(handle->fd); handle->fd = -1; return 0;
    }
    handle->ptr = mmap(NULL, sizeof(SharedCoreMemory),
                       PROT_READ | PROT_WRITE, MAP_SHARED, handle->fd, 0);
    if (handle->ptr == MAP_FAILED) {
        perror("mmap attach"); handle->ptr = NULL;
        close(handle->fd); handle->fd = -1; return 0;
    }
    if (handle->ptr->magic != CORE_SHM_MAGIC ||
        handle->ptr->version != CORE_SHM_VERSION ||
        !handle->ptr->ready) {
        fprintf(stderr, "Shared memory is not initialized by Core.\\n");
        core_shm_close(handle); return 0;
    }
    return 1;
}

void core_shm_lock(SharedCoreMemory *shared)
{
    if (shared) pthread_mutex_lock(&shared->mutex);
}
void core_shm_unlock(SharedCoreMemory *shared)
{
    if (shared) pthread_mutex_unlock(&shared->mutex);
}
void core_shm_close(CoreShmHandle *handle)
{
    if (!handle) return;
    if (handle->ptr) munmap(handle->ptr, sizeof(SharedCoreMemory));
    if (handle->fd != -1) close(handle->fd);
    handle->ptr = NULL; handle->fd = -1;
}
void core_shm_unlink(void)
{
    shm_unlink(CORE_SHM_NAME);
}
