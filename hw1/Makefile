# top-level Makefile 
# fix BASE_DIR

BASE_DIR = .
SRC_DIR = $(BASE_DIR)/src
INPUT_DIR = $(BASE_DIR)/data
OUTPUT_DIR = $(BASE_DIR)/result

PROG = maxcut
INPUT = $(INPUT_DIR)/$(PROG).in
OUTPUT = $(OUTPUT_DIR)/$(PROG).out

FLAGS = 'PROG=$(PROG)'

all: $(PROG)
$(PROG):
	$(MAKE) $(FLAGS) -C $(SRC_DIR)

.PHONY: test clean

test: $(PROG)
	mkdir -p $(OUTPUT_DIR)
	./$(PROG) $(INPUT) $(OUTPUT)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(PROG)
