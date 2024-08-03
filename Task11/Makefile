CC := gcc
BIN := task11_1 task11_2 task11_3
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task11_1.o task11_2.o task11_3.o threadfunc.o
CFLAGS := -g
LDFLAGS := -pthread
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 3

1: task11_1.o
		$(CC) task11_1.o -o task11_1 $(LDFLAGS) 

2: task11_2.o
		$(CC) task11_2.o -o task11_2 $(LDFLAGS) 	

3: threadfunc.o task11_3.o		
		$(CC) threadfunc.o task11_3.o -o task11_3 $(LDFLAGS) 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS)

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)