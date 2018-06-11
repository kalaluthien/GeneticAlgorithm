# sub-level Makefile

ROOT_DIR = ..
INCLUDE_DIR = $(ROOT_DIR)/include

CC = gcc

CFLAGS = -g -pg -std=c99 -Wall -I$(INCLUDE_DIR)
LDFLAGS = -pg
OPTS = -O2

SRCS = main.c graph.c sol.c ga.c tabu.c fm.c timer.c
OBJS = $(SRCS:%.c=%.o)

all: $(OBJS)
	$(CC) -o $(ROOT_DIR)/$(PROG) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS)
