CC := gcc
BIN := task1 task2 
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task1.o task2.o
CFLAGS := -g
PFLAG := -lrt
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 

1: task1.o
		$(CC) task1.o -o task1 		

2: task2.o
		$(CC) task2.o -o task2 
		  
%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS)

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)