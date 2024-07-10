CC := gcc
BIN := task8
SRC := task8.c
LIB_SRC := add.c sub.c mul.c div.c
HDR := $(ls | grep *.h)
OBJ := task8.o
LIB_NAME := libcalc.so
LIB_OBJ := add.o div.o mul.o sub.o
CFLAGS := -g
LDFLAGS := -L. -lcalc

.PHONY: clean start

$(BIN): $(OBJ) $(LIB_NAME)
		$(CC) $< -o $@ $(LDFLAGS)

$(LIB_NAME): $(LIB_OBJ)
		$(CC) -shared $^ -o $@

$(LIB_OBJ): $(LIB_SRC)
		$(CC) $^ -fPIC -c 

$(OBJ): $(SRC)
		$(CC) $^ -c 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ) $(LIB_OBJ) $(LIB_NAME)

start: 
		LD_LIBRARY_PATH=$(PWD) ./$(BIN)