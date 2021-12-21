# $@ : repalced by name of target
# $< : replaced by name of first prerequisite
# $^ : replaced by names of all prerequisites
# := : evaluated once
# =  : evaluated each time
# %. : pattern thingy (one to one?)
# $(patsubst pattern,replacement,text) : finds words in text that match pattern and replaces them with replacement
# $(wildcard) : (many to many)


CC=gcc
CFLAGS=-g -Wall -std=c99
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BINDIR=bin
BIN=$(BINDIR)/main

LIBDIR=libs
#LIB=$(LIBDIR)/

TEST=tests
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%.o, $(TESTS))

all:$(BIN)

# DEPS=$(patsubst %.o, %.d, $(OBJS))
# -include $(DEPS)
# DEPFLAGS=-MMD -MF $(@:.o=.d)

# release: CFLAGS=-Wall -02 -DNDEBUG
# release: clean
# release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST)/bin/%: $(TEST)/%.C
	$(CC) $(CFLAGS) $< $(OBJS) -o $@ 

$(TEST)/bin:
	mkdir $@

$(OBJ):
	mkdir $@

$(LIBDIR):
	mkdir $@

test: $(LIB) $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

clean:
	$(RM) -r $(BINDIR)/* $(OBJ)/*

