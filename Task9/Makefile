CC := gcc
BIN := task9_1 task9_2 
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task9_1.o task9_2.o ncursesdirmanager.o
CFLAGS := -g
LDFLAGS := -lncurses

.PHONY: clean

all: task9_1 task9_2

task9_1: task9_1.o
		$(CC) task9_1.o -o task9_1

task9_2: task9_2.o
		$(CC) ncursesdirmanager.c -c -o ncursesdirmanager.o
		$(CC) task9_2.o ncursesdirmanager.o -o task9_2 $(LDFLAGS)

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)