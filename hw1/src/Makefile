# sub-level Makefile

ROOT_DIR = ..
INCLUDE_DIR = $(ROOT_DIR)/include

CC = gcc

CFLAGS = -g -std=c99 -Wall -I$(INCLUDE_DIR)
LDFLAGS = -lm
OPTS = -O3

SRCS = main.c parser.c graph.c ga.c timer.c
OBJS = $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) -o $(ROOT_DIR)/$(PROG) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS)
