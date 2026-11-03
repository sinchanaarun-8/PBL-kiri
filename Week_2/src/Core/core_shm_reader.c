#include <stdio.h>
#include "shared_memory.h"

/* Small independent process used to prove that another process can read Core's state. */
int main(void)
{
    CoreShmHandle handle;
    CoreState snapshot;

    if (!core_shm_attach(&handle)) return 1;
    core_shm_lock(handle.ptr);
    snapshot = handle.ptr->state;
    printf("SHM reader attached successfully\\n");
    printf("Updates: %lu\\n", handle.ptr->update_count);
    core_shm_unlock(handle.ptr);

    core_print_state(&snapshot);
    core_shm_close(&handle);
    return 0;
}
