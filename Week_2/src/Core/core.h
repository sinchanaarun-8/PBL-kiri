#ifndef CORE_H
#define CORE_H

#include <stddef.h>

#include "cpu.h"
#include "memory.h"
#include "stack.h"
#include "queue.h"

#define CORE_PROGRAM_SIZE 32
#define CORE_INSTRUCTION_SIZE 64

typedef enum
{
    CORE_CMD_INVALID = 0,
    CORE_CMD_LOAD,
    CORE_CMD_RUN,
    CORE_CMD_STOP,
    CORE_CMD_RESET,
    CORE_CMD_EXIT
} CoreCommand;


typedef struct
{
    CPU cpu;
    Memory memory;
    Stack stack;
    Queue queue;

    char program[CORE_PROGRAM_SIZE][CORE_INSTRUCTION_SIZE];
    size_t program_size;

} CoreState;


void core_init(CoreState *core);

void core_reset(CoreState *core);

int core_load_default_program(CoreState *core);

int core_execute(
    CoreState *core,
    const char *instruction
);

int core_run(CoreState *core);

int core_stop(CoreState *core);

int core_process_command(
    CoreState *core,
    CoreCommand command
);

void core_print_state(
    const CoreState *core
);

#endif