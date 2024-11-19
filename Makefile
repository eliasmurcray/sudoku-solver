CC ?= gcc
CFLAGS = -std=c89 -Ofast -Wall -Wextra -Iinclude/
BUILD = build
SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=$(BUILD)/%.o)
LIBS = $(shell pkg-config sdl3 sdl3-ttf --cflags --libs)

.PHONY: all

all: $(BUILD)/main

$(BUILD)/main: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	@mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(BUILD)
