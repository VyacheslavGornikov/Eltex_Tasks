CC := gcc
BIN := task7
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task7.o
LIB_NAME := libcalc.a
LIB_OBJ := add.o div.o mul.o sub.o
CFLAGS := -g
LDFLAGS := -L. -lcalc

.PHONY: clean

$(BIN): $(OBJ) $(LIB_NAME)
		$(CC) $< -o $@ $(LDFLAGS)

$(LIB_NAME): $(LIB_OBJ)
		ar rc $@ $^

%.o: %.c
		$(CC) $< -c -o $@

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ) $(LIB_OBJ) $(LIB_NAME)