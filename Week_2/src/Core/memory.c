#include <stdio.h>
#include <string.h>

#include "memory.h"


void memory_init(Memory *memory)
{
    if (memory == NULL)
    {
        return;
    }

    memset(
        memory->data,
        0,
        sizeof(memory->data)
    );
}


void memory_reset(Memory *memory)
{
    memory_init(memory);
}


int memory_write(
    Memory *memory,
    int location,
    int data)
{
    if (memory == NULL)
    {
        return 0;
    }

    if (location < 0 ||
        location >= MEMORY_CAPACITY)
    {
        printf(
            "Memory: invalid address %d\n",
            location
        );

        return 0;
    }

    memory->data[location] = data;

    return 1;
}


int memory_read(
    const Memory *memory,
    int location)
{
    if (memory == NULL)
    {
        return -1;
    }

    if (location < 0 ||
        location >= MEMORY_CAPACITY)
    {
        printf(
            "Memory: invalid address %d\n",
            location
        );

        return -1;
    }

    return memory->data[location];
}