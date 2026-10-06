#include <stdio.h>
#include <string.h>

#include "core.h"


static void clear_program(CoreState *core)
{
    size_t i;

    for (i = 0; i < CORE_PROGRAM_SIZE; i++)
    {
        core->program[i][0] = '\0';
    }

    core->program_size = 0;
}


void core_init(CoreState *core)
{
    if (core == NULL)
    {
        return;
    }

    cpu_init(&core->cpu);
    memory_init(&core->memory);
    stack_init(&core->stack);
    queue_init(&core->queue);

    clear_program(core);
}


void core_reset(CoreState *core)
{
    core_init(core);
}


int core_load_default_program(CoreState *core)
{
    static const char *default_program[] =
    {
        "LOAD 10",
        "ADD 20",
        "PUSH 30",
        "POP",
        "HALT"
    };

    size_t count;
    size_t i;

    if (core == NULL)
    {
        return 0;
    }

    count =
        sizeof(default_program) /
        sizeof(default_program[0]);

    if (count > CORE_PROGRAM_SIZE)
    {
        return 0;
    }

    clear_program(core);

    for (i = 0; i < count; i++)
    {
        strncpy(
            core->program[i],
            default_program[i],
            CORE_INSTRUCTION_SIZE - 1
        );

        core->program[i][CORE_INSTRUCTION_SIZE - 1] =
            '\0';
    }

    core->program_size = count;

    cpu_reset(&core->cpu);

    return 1;
}


int core_execute(
    CoreState *core,
    const char *instruction)
{
    char command[16];
    int value;
    int address;
    int result;

    if (core == NULL ||
        instruction == NULL)
    {
        return 0;
    }

    command[0] = '\0';

    if (sscanf(
            instruction,
            "%15s",
            command) != 1)
    {
        return 0;
    }


    /*
     * CPU instructions
     */
    if (strcmp(command, "LOAD") == 0 ||
        strcmp(command, "ADD") == 0 ||
        strcmp(command, "SUB") == 0 ||
        strcmp(command, "MUL") == 0 ||
        strcmp(command, "DIV") == 0 ||
        strcmp(command, "HALT") == 0)
    {
        return cpu_execute(
            &core->cpu,
            instruction
        );
    }


    /*
     * Memory instructions
     */
    if (strcmp(command, "STORE") == 0)
    {
        if (sscanf(
                instruction,
                "%15s %d %d",
                command,
                &address,
                &value) != 3)
        {
            return 0;
        }

        return memory_write(
            &core->memory,
            address,
            value
        );
    }


    if (strcmp(command, "READ") == 0)
    {
        if (sscanf(
                instruction,
                "%15s %d",
                command,
                &address) != 2)
        {
            return 0;
        }

        result = memory_read(
            &core->memory,
            address
        );

        if (result == -1)
        {
            return 0;
        }

        printf(
            "Memory[%d] = %d\n",
            address,
            result
        );

        return 1;
    }


    /*
     * Stack instructions
     */
    if (strcmp(command, "PUSH") == 0)
    {
        if (sscanf(
                instruction,
                "%15s %d",
                command,
                &value) != 2)
        {
            return 0;
        }

        return stack_push(
            &core->stack,
            value
        );
    }


    if (strcmp(command, "POP") == 0)
    {
        if (!stack_pop(
                &core->stack,
                &result))
        {
            return 0;
        }

        printf(
            "Popped %d\n",
            result
        );

        return 1;
    }


    if (strcmp(command, "PEEK") == 0)
    {
        if (!stack_peek(
                &core->stack,
                &result))
        {
            return 0;
        }

        printf(
            "Stack top = %d\n",
            result
        );

        return 1;
    }


    /*
     * Queue instructions
     */
    if (strcmp(command, "ENQUEUE") == 0)
    {
        if (sscanf(
                instruction,
                "%15s %d",
                command,
                &value) != 2)
        {
            return 0;
        }

        return queue_enqueue(
            &core->queue,
            value
        );
    }


    if (strcmp(command, "DEQUEUE") == 0)
    {
        if (!queue_dequeue(
                &core->queue,
                &result))
        {
            return 0;
        }

        printf(
            "Dequeued %d\n",
            result
        );

        return 1;
    }


    if (strcmp(command, "QPEEK") == 0)
    {
        if (!queue_peek(
                &core->queue,
                &result))
        {
            return 0;
        }

        printf(
            "Queue front = %d\n",
            result
        );

        return 1;
    }


    printf(
        "Core: Unknown instruction: %s\n",
        command
    );

    return 0;
}


int core_run(CoreState *core)
{
    size_t i;

    if (core == NULL ||
        core->program_size == 0)
    {
        return 0;
    }

    core->cpu.active = 1;

    for (i = 0;
         i < core->program_size &&
         core->cpu.active;
         i++)
    {
        if (!core_execute(
                core,
                core->program[i]))
        {
            core->cpu.active = 0;
            return 0;
        }
    }

    return 1;
}


int core_stop(CoreState *core)
{
    if (core == NULL)
    {
        return 0;
    }

    cpu_stop(&core->cpu);

    return 1;
}


int core_process_command(
    CoreState *core,
    CoreCommand command)
{
    if (core == NULL)
    {
        return 0;
    }

    switch (command)
    {
        case CORE_CMD_LOAD:

            return core_load_default_program(core);


        case CORE_CMD_RUN:

            return core_run(core);


        case CORE_CMD_STOP:

            return core_stop(core);


        case CORE_CMD_RESET:

            core_reset(core);
            return 1;


        case CORE_CMD_EXIT:

            core_stop(core);
            return 1;


        default:

            return 0;
    }
}


void core_print_state(
    const CoreState *core)
{
    if (core == NULL)
    {
        return;
    }

    printf("\n");
    printf("========== CORE STATE ==========\n");

    printf(
        "ACC           : %d\n",
        core->cpu.acc
    );

    printf(
        "PC            : %d\n",
        core->cpu.pc
    );

    printf(
        "CPU Active    : %s\n",
        core->cpu.active ? "YES" : "NO"
    );

    printf(
        "Program Size  : %zu\n",
        core->program_size
    );

    printf(
        "Stack Items   : %d\n",
        core->stack.top + 1
    );

    printf(
        "Queue Items   : %d\n",
        core->queue.count
    );

    printf(
        "Memory[0]     : %d\n",
        core->memory.data[0]
    );

    printf(
        "================================\n"
    );
}