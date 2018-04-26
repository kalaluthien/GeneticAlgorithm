# top-level Makefile 

ROOT_DIR = .
SRC_DIR = $(ROOT_DIR)/src

PROG = maxcut
INPUT = $(PROG).in
OUTPUT = $(PROG).out

FLAGS = 'PROG=$(PROG)'

all:
	$(MAKE) $(FLAGS) -C $(SRC_DIR)

.PHONY: run clean

run: all
	./$(PROG) $(INPUT) $(OUTPUT)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(PROG) $(INPUT) $(OUTPUT)
