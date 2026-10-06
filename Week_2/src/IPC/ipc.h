#ifndef IPC_H
#define IPC_H

#include <mqueue.h>

#define UI_TO_CORE_QUEUE "/pbl_ui_to_core"
#define CORE_TO_UI_QUEUE "/pbl_core_to_ui"
#define CORE_TO_LOGGER_QUEUE "/pbl_core_to_logger"

#define IPC_MSG_COMMAND 1
#define IPC_MSG_RESPONSE 2
#define IPC_MSG_LOG 3
#define IPC_CMD_LOAD 1
#define IPC_CMD_RUN 2
#define IPC_CMD_STOP 3
#define IPC_CMD_RESET 4
#define IPC_CMD_EXIT 5

#define IPC_TEXT_SIZE 128

typedef struct
{
    int type;
    int command;
    int status;
    char text[IPC_TEXT_SIZE];
} IPCMessage;

mqd_t ipc_open_queue(const char *name, int flags, int create);

int ipc_send(
    mqd_t queue,
    const IPCMessage *message
);

int ipc_receive(
    mqd_t queue,
    IPCMessage *message
);

void ipc_remove_queues(void);

#endif
