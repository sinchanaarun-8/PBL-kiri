CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lrt

BUILD_DIR = build

CORE_SRCS = \
	Week_2/src/Core/core_process.c \
	Week_2/src/Core/core.c \
	Week_2/src/Core/cpu.c \
	Week_2/src/Core/memory.c \
	Week_2/src/Core/queue.c \
	Week_2/src/Core/stack.c \
	Week_2/src/IPC/ipc.c

LOGGER_SRCS = \
	Week_2/src/logger/logger_process.c \
	Week_2/src/logger/logger.c \
	Week_2/src/IPC/ipc.c

UI_SRCS = \
	Week_2/src/ui/ui.c \
	Week_2/src/IPC/ipc.c

MAIN_SRC = Week_2/src/main.c

.PHONY: all clean

all: $(BUILD_DIR)/core_process \
	$(BUILD_DIR)/logger_process \
	$(BUILD_DIR)/ui_process \
	$(BUILD_DIR)/pbl_simulator

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/core_process: $(CORE_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CORE_SRCS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/logger_process: $(LOGGER_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LOGGER_SRCS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/ui_process: $(UI_SRCS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(UI_SRCS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/pbl_simulator: $(MAIN_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(MAIN_SRC) -o $@

clean:
	rm -rf $(BUILD_DIR)
