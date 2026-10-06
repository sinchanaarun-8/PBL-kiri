#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>
#include <fcntl.h>

#include "ui.h"
#include "../IPC/ipc.h"

/* Display the simulator header and current status */
static void display_header(void)
{
    printf("\n========================================\n");
    printf("          PBL PROCESS SIMULATOR\n");
    printf("========================================\n\n");

    printf("Status: READY\n\n");
}


/* Display the available simulator commands */
static void display_menu(void)
{
    printf("1. Load Program\n");
    printf("2. Run Program\n");
    printf("3. Stop Program\n");
    printf("4. Reset Simulator\n");
    printf("5. Exit\n");
}


/* Convert menu choice into a logical UI command */
static UICommand get_command(int choice)
{
    switch (choice)
    {
        case 1:
            return UI_CMD_LOAD;

        case 2:
            return UI_CMD_RUN;

        case 3:
            return UI_CMD_STOP;

        case 4:
            return UI_CMD_RESET;

        case 5:
            return UI_CMD_EXIT;

        default:
            return UI_CMD_INVALID;
    }
}


/* Convert a logical command into readable text */
static const char *command_to_string(UICommand command)
{
    switch (command)
    {
        case UI_CMD_LOAD:
            return "LOAD";

        case UI_CMD_RUN:
            return "RUN";

        case UI_CMD_STOP:
            return "STOP";

        case UI_CMD_RESET:
            return "RESET";

        case UI_CMD_EXIT:
            return "EXIT";

        default:
            return "INVALID";
    }
}


/*
 * Read and validate user input.
 *
 * Returns:
 *   1  -> valid integer received
 *   0  -> input stream closed
 *  -1  -> invalid input
 */
static int read_choice(int *choice)
{
    char input[64];
    char *endptr;
    long value;

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return 0;
    }

    errno = 0;
    endptr = input;

    while (isspace((unsigned char)*endptr))
    {
        endptr++;
    }

    if (*endptr == '\0')
    {
        return -1;
    }

    value = strtol(endptr, &endptr, 10);

    while (isspace((unsigned char)*endptr))
    {
        endptr++;
    }

    if (errno == ERANGE ||
        value < INT_MIN ||
        value > INT_MAX ||
        *endptr != '\0')
    {
        return -1;
    }

    *choice = (int)value;

    return 1;
}


int main(void)
{
    int choice;
    int input_status;
    UICommand command;

    mqd_t ui_to_core;
    mqd_t core_to_ui;
    IPCMessage message;
    IPCMessage response;
    ui_to_core = ipc_open_queue(
        UI_TO_CORE_QUEUE,
        O_WRONLY,
        0
    );

    if (ui_to_core == (mqd_t)-1)
    {
        perror("Failed to open UI -> Core queue");
        return 1;
    }

    core_to_ui = ipc_open_queue(
        CORE_TO_UI_QUEUE,
        O_RDONLY,
        0
    );

    if (core_to_ui == (mqd_t)-1)
    {
        perror("Failed to open Core -> UI queue");
        mq_close(ui_to_core);
        return 1;
    }

    while (1)
    {
        display_header();
        display_menu();

        printf("\nEnter choice: ");
        fflush(stdout);

        input_status = read_choice(&choice);

        /*
         * Handle closed input.
         */
        if (input_status == 0)
        {
            printf("\nInput closed. Exiting simulator...\n");
            break;
        }

        /*
         * Handle non-numeric input.
         */
        if (input_status < 0)
        {
            printf("\nInvalid input.\n");
            printf("Please enter a number from 1-5.\n");
            continue;
        }

        /*
         * Convert the menu choice into a logical command.
         */
        command = get_command(choice);

        /*
         * Handle invalid menu choices.
         */
        if (command == UI_CMD_INVALID)
        {
            printf("\nInvalid choice.\n");
            printf("Please select 1-5.\n");
            continue;
        }

        /*
         * Display the logical command.
         *
         * In a later version, this is where the command
         * can be passed to the Core Process through IPC.
         */
        printf("\nCommand: %s\n", command_to_string(command));
        message.type = IPC_MSG_COMMAND;
        message.command = command;
        message.status = 0;
        message.text[0] = '\0';

        if (!ipc_send(ui_to_core, &message))
        {
            perror("Failed to send command to Core");
            break;
        }

        if (!ipc_receive(core_to_ui, &response))
        {
            perror("Failed to receive response from Core");
            break;
        }

        printf(
            "Core response: %s\n",
            response.text
        );

        /*
         * Exit the simulator.
         */
        if (command == UI_CMD_EXIT)
        {
            printf("Exiting simulator...\n");
            break;
        }
    }
    mq_close(ui_to_core);
    mq_close(core_to_ui);

    return 0;
}
