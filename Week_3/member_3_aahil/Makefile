
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude

TARGET = build/scheduler
SOURCES = src/main.c src/basic_scheduling.c src/srtf.c src/round_robin.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SOURCES) include/scheduling.h
	mkdir -p build
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
