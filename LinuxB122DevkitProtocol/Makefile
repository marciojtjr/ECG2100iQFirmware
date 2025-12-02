# Makefile for Linux BT122 DevKit Protocol Monitor

CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
CFLAGS_DEBUG = -Wall -Wextra -g -O0 -std=c99 -DDEBUG
LDFLAGS =
TARGET = bt122_monitor
SOURCES = main.c bgapi_parser.c bgapi_commands.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean install debug

all: $(TARGET)

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

install: $(TARGET)
	install -m 755 $(TARGET) /usr/local/bin/

run: $(TARGET)
	./$(TARGET)

# Show help
help:
	@echo "Available targets:"
	@echo "  all     - Build the program (default)"
	@echo "  debug   - Build with debug symbols for GDB"
	@echo "  clean   - Remove built files"
	@echo "  install - Install to /usr/local/bin (requires sudo)"
	@echo "  run     - Build and run the program"
	@echo "  help    - Show this help message"
