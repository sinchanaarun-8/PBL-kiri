#include <stdio.h>
#include <string.h>

#define MEMORY_SIZE 100
#define STACK_SIZE 50
#define QUEUE_SIZE 50

/* ---------------- CPU ---------------- */

typedef struct
{
    int program_counter;
    int accumulator;
    int halted;
} CPU;

/* ---------------- STACK ---------------- */

typedef struct
{
    int data[STACK_SIZE];
    int top;
} Stack;

/* ---------------- QUEUE ---------------- */

typedef struct
{
    int data[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;

/* ---------------- CORE STATE ---------------- */

int memory[MEMORY_SIZE];
CPU cpu;
Stack stack;
Queue queue;

/* ---------------- MEMORY ---------------- */

void memory_init(void)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }
}

void memory_write(int address, int value)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("[ERROR] Invalid memory address: %d\n", address);
        return;
    }

    memory[address] = value;
}

int memory_read(int address)
{
    if (address < 0 || address >= MEMORY_SIZE)
    {
        printf("[ERROR] Invalid memory address: %d\n", address);
        return 0;
    }

    return memory[address];
}

/* ---------------- STACK ---------------- */

void stack_init(void)
{
    stack.top = -1;
}

void stack_push(int value)
{
    if (stack.top >= STACK_SIZE - 1)
    {
        printf("[ERROR] Stack overflow\n");
        return;
    }

    stack.data[++stack.top] = value;
}

int stack_pop(void)
{
    if (stack.top < 0)
    {
        printf("[ERROR] Stack underflow\n");
        return 0;
    }

    return stack.data[stack.top--];
}

/* ---------------- QUEUE ---------------- */

void queue_init(void)
{
    queue.front = 0;
    queue.rear = -1;
    queue.count = 0;
}

void queue_enqueue(int value)
{
    if (queue.count >= QUEUE_SIZE)
    {
        printf("[ERROR] Queue overflow\n");
        return;
    }

    queue.rear = (queue.rear + 1) % QUEUE_SIZE;
    queue.data[queue.rear] = value;
    queue.count++;
}

int queue_dequeue(void)
{
    int value;

    if (queue.count == 0)
    {
        printf("[ERROR] Queue underflow\n");
        return 0;
    }

    value = queue.data[queue.front];
    queue.front = (queue.front + 1) % QUEUE_SIZE;
    queue.count--;

    return value;
}

/* ---------------- CPU ---------------- */

void cpu_init(void)
{
    cpu.program_counter = 0;
    cpu.accumulator = 0;
    cpu.halted = 0;
}

/* ---------------- CORE ---------------- */

void core_init(void)
{
    cpu_init();
    memory_init();
    stack_init();
    queue_init();
}

/* ---------------- DEMONSTRATION ---------------- */

void run_core_demo(void)
{
    printf("\n=====================================\n");
    printf("       PBL CORE PROCESS\n");
    printf("=====================================\n");

    printf("\n[CPU] Loading value 10...\n");
    cpu.accumulator = 10;
    cpu.program_counter++;

    printf("[CPU] ACC = %d\n", cpu.accumulator);

    printf("\n[CPU] Adding 20...\n");
    cpu.accumulator += 20;
    cpu.program_counter++;

    printf("[CPU] ACC = %d\n", cpu.accumulator);

    printf("\n[MEMORY] Storing ACC at address 10...\n");
    memory_write(10, cpu.accumulator);

    printf("[MEMORY] Memory[10] = %d\n", memory_read(10));

    printf("\n[STACK] Pushing ACC...\n");
    stack_push(cpu.accumulator);

    printf("[STACK] Popping value...\n");
    cpu.accumulator = stack_pop();

    printf("[STACK] ACC = %d\n", cpu.accumulator);

    printf("\n[QUEUE] Enqueueing values...\n");
    queue_enqueue(10);
    queue_enqueue(20);
    queue_enqueue(30);

    printf("[QUEUE] Dequeued value = %d\n", queue_dequeue());

    printf("\n[CPU] Halting...\n");
    cpu.halted = 1;

    printf("\n=====================================\n");
    printf("       CORE EXECUTION COMPLETE\n");
    printf("=====================================\n");

    printf("Program Counter : %d\n", cpu.program_counter);
    printf("Accumulator     : %d\n", cpu.accumulator);
    printf("Memory[10]      : %d\n", memory_read(10));
    printf("CPU Status      : %s\n", cpu.halted ? "HALTED" : "RUNNING");
}

/* ---------------- MAIN ---------------- */

int main(void)
{
    core_init();

    run_core_demo();

    return 0;
}
