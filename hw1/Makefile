# top-level Makefile 
# fix BASE_DIR

BASE_DIR = .
SRC_DIR = $(BASE_DIR)/src
INPUT_DIR = $(BASE_DIR)/data
OUTPUT_DIR = $(BASE_DIR)/result

PROGRAM = maxcut
INPUT = $(INPUT_DIR)/input.txt
OUTPUT = $(OUTPUT_DIR)/output.txt

FLAGS = 'PROGRAM=$(PROGRAM)'

all: $(PROGRAM)
$(PROGRAM):
	$(MAKE) $(FLAGS) -C $(SRC_DIR)

.PHONY: test clean

test: $(PROGRAM)
	mkdir -p $(OUTPUT_DIR)
	./$(PROGRAM) $(INPUT) $(OUTPUT)

clean:
	$(MAKE) -C $(SRC_DIR) clean
	rm -f $(PROGRAM)
