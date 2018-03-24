# sub-level Makefile
# fix BASE_DIR

BASE_DIR = ..
INCLUDE_DIR = $(BASE_DIR)/include

CC = gcc

CFLAGS = -g -std=c99 -Wall -I$(INCLUDE_DIR)
LDFLAGS =
OPTS = -O2

SRCS = main.c parser.c timer.c graph.c list.c
OBJS = $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(BASE_DIR)/$(PROGRAM)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJS)
