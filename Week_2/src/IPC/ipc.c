#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "ipc.h"

static struct mq_attr ipc_attributes(void)
{
    struct mq_attr attr;

    memset(&attr, 0, sizeof(attr));

    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(IPCMessage);

    return attr;
}

mqd_t ipc_open_queue(const char *name, int flags, int create)
{
    if (create)
    {
        struct mq_attr attr = ipc_attributes();

        return mq_open(
            name,
            flags | O_CREAT,
            0666,
            &attr
        );
    }

    return mq_open(name, flags);
}

int ipc_send(
    mqd_t queue,
    const IPCMessage *message)
{
    if (queue == (mqd_t)-1 || message == NULL)
    {
        return 0;
    }

    return mq_send(
        queue,
        (const char *)message,
        sizeof(IPCMessage),
        0
    ) == 0;
}

int ipc_receive(
    mqd_t queue,
    IPCMessage *message)
{
    if (queue == (mqd_t)-1 || message == NULL)
    {
        return 0;
    }

    return mq_receive(
        queue,
        (char *)message,
        sizeof(IPCMessage),
        NULL
    ) == (ssize_t)sizeof(IPCMessage);
}

void ipc_remove_queues(void)
{
    mq_unlink(UI_TO_CORE_QUEUE);
    mq_unlink(CORE_TO_UI_QUEUE);
    mq_unlink(CORE_TO_LOGGER_QUEUE);
}

