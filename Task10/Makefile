CC := gcc
BIN := task10_1 task10_2 task10_3
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task10_1.o task10_2.o task10_3.o ncursesdirmanager.o
CFLAGS := -g
LDFLAGS := -lncurses

.PHONY: clean

all: 1 2 3

1: task10_1.o
		$(CC) task10_1.o -o task10_1

2: task10_2.o
		$(CC) task10_2.o -o task10_2		

3: ncursesdirmanager.o task10_3.o		
		$(CC) task10_3.o ncursesdirmanager.o -o task10_3 $(LDFLAGS)

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)