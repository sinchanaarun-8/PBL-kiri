#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_CAPACITY 100

typedef struct
{
    int data[MEMORY_CAPACITY];
} Memory;

void memory_init(Memory *memory);
void memory_reset(Memory *memory);

int memory_write(
    Memory *memory,
    int location,
    int data
);

int memory_read(
    const Memory *memory,
    int location
);

#endif