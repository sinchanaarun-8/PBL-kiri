#ifndef UI_H
#define UI_H

typedef enum
{
    UI_CMD_INVALID = 0,
    UI_CMD_LOAD,
    UI_CMD_RUN,
    UI_CMD_STOP,
    UI_CMD_RESET,
    UI_CMD_EXIT
} UICommand;

#endif