CC = gcc
CFLAGS = -Wall -Wextra -g -O0 -Iinclude -std=c99
LDLIBS =
SRC = $(wildcard raspberry/src/*.c)
OBJ = $(SRC:raspberry/src/%.c=raspberry/build/%.o)
BIN = projet_4

C_INC = $(wildcard common/*.h)
R_INC = $(C_INC:common/%.h=raspberry/include/%.h)

C_SRC = $(wildcard common/*.c)
R_SRC = $(C_SRC:common/%.c=raspberry/src/%.c)

.PHONY : all sync

all: $(BIN)

$(BIN): sync $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	@# $^ pour appeler tous les .o présents dans build/

sync: $(R_INC) $(R_SRC) 

raspberry/include/%.h : common/%.h
	cp $^ $@

raspberry/src/%.c : common/%.c
	cp $^ $@

raspberry/build/%.o: raspberry/src/%.c
	@mkdir -p raspberry/build
	$(CC) $(CFLAGS) -c $^ -o $@
	@# $^ pour appeler tous les .c présents dans source/
