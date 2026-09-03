CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -Iraspberry/include -std=c99
LDLIBS =
SRC = $(wildcard raspberry/src/*.c)

C_INC = $(wildcard common/*.h)
R_INC = $(C_INC:common/%.h=raspberry/include/%.h)
C_SRC = $(wildcard common/*.c)
R_SRC = $(C_SRC:common/%.c=raspberry/src/%.c)

ALL_SRC = $(filter-out $(R_SRC),$(SRC)) $(R_SRC)
OBJ = $(ALL_SRC:raspberry/src/%.c=raspberry/build/%.o)

BIN = projet_4

.PHONY : all sync

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	@# $^ pour appeler tous les .o présents dans build/

sync: $(R_INC) $(R_SRC) 

raspberry/include/%.h : common/%.h
	@mkdir -p raspberry/include/
	cp $^ $@

raspberry/src/%.c : common/%.c
	@mkdir -p raspberry/src/
	cp $^ $@

raspberry/build/%.o: raspberry/src/%.c | sync
	@mkdir -p raspberry/build
	$(CC) $(CFLAGS) -c $^ -o $@
	@# $^ pour appeler tous les .c présents dans src/
