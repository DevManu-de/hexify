CC := gcc
CFLAGS := -Wall -Wextra -g
LFLAGS := -lm -lxmemtools -lncurses
OUTPUT := output
SRC := src
INCLUDE := include lib
LIB := lib

RM := rm -rf
MD := mkdir -p
CTAGS := ctags
CTAGSFLAGS := -R
CTAGSDIRECTORIES := $(SRC) $(INCLUDE) $(LIB)

MAIN := hexify
SOURCEDIRS := $(shell find $(SRC) -type d)
INCLUDEDIRS := $(shell find $(INCLUDE) -type d)
LIBDIRS := $(shell find $(LIB) -type d)

INCLUDES := $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))
LIBS := $(patsubst %,-L%, $(LIBDIRS:%/=%))
SOURCES := $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))
OBJECTS := $(SOURCES:.c=.o)
OUTPUTMAIN := $(OUTPUT)/$(MAIN)

all: $(OUTPUT) $(MAIN)
	@echo Compilation successful

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(MAKE) -C $(LIB)/xmemtools -i
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)
	$(CTAGS) $(CTAGSFLAGS) $(CTAGSDIRECTORIES)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run: all
	./$(OUTPUTMAIN) -f Makefile
	@echo Execution successful

.PHONY: clean
clean:
	$(MAKE) -C $(LIB)/xmemtools clean
	$(RM) $(OUTPUT) $(OBJECTS)
	@echo Clean successful
