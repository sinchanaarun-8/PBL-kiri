#include <stdio.h>
#include <string.h>

#include "cpu.h"


void cpu_init(CPU *cpu)
{
    if (cpu == NULL)
    {
        return;
    }

    cpu->acc = 0;
    cpu->pc = 0;
    cpu->active = 1;
}


void cpu_reset(CPU *cpu)
{
    cpu_init(cpu);
}


void cpu_stop(CPU *cpu)
{
    if (cpu == NULL)
    {
        return;
    }

    cpu->active = 0;
}


int cpu_execute(CPU *cpu, const char *instruction)
{
    char command[16];
    int number = 0;

    if (cpu == NULL || instruction == NULL)
    {
        return 0;
    }

    command[0] = '\0';

    if (sscanf(
            instruction,
            "%15s %d",
            command,
            &number) < 1)
    {
        return 0;
    }

    if (strcmp(command, "LOAD") == 0)
    {
        cpu->acc = number;
    }
    else if (strcmp(command, "ADD") == 0)
    {
        cpu->acc += number;
    }
    else if (strcmp(command, "SUB") == 0)
    {
        cpu->acc -= number;
    }
    else if (strcmp(command, "MUL") == 0)
    {
        cpu->acc *= number;
    }
    else if (strcmp(command, "DIV") == 0)
    {
        if (number == 0)
        {
            printf("CPU: Error - division by zero\n");
            return 0;
        }

        cpu->acc /= number;
    }
    else if (strcmp(command, "HALT") == 0)
    {
        cpu->active = 0;
    }
    else
    {
        printf("CPU: Invalid instruction: %s\n", command);
        return 0;
    }

    cpu->pc++;

    return 1;
}