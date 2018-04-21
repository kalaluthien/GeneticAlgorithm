# top-level Makefile 

ROOT_DIR = .
SRC_DIR = $(ROOT_DIR)/src
INPUT_DIR = $(ROOT_DIR)/data
OUTPUT_DIR = $(ROOT_DIR)/result

PROG = maxcut
INPUT = $(PROG).in
OUTPUT = $(PROG).out

FLAGS = 'PROG=$(PROG)'

all:
	$(MAKE) $(FLAGS) -C $(SRC_DIR)

.PHONY: run clean

run: all
	cp -f $(INPUT_DIR)/$(INPUT) $(INPUT)
	./$(PROG) $(INPUT) $(OUTPUT)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(PROG) $(INPUT) $(OUTPUT)
