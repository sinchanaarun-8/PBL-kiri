#ifndef CPU_H
#define CPU_H

typedef struct
{
    int acc;
    int pc;
    int active;
} CPU;

void cpu_init(CPU *cpu);
void cpu_reset(CPU *cpu);
void cpu_stop(CPU *cpu);

int cpu_execute(CPU *cpu, const char *instruction);

#endif