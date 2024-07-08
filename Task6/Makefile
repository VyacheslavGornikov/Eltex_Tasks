CC := gcc
BIN := task6
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := phonebook.o task6.o
CFLAGS := -g

.PHONY: clean

$(BIN): $(OBJ)
		$(CC) $^ -o $@

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)