CC := gcc
BIN := task12_1 task12_2_cli task12_2_serv task12_3
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := task12_1.o task12_2_cli.o task12_2_serv.o task12_3.o ncursesdirmanager.o ncursescmd.o 
CFLAGS := -g
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 3

1: task12_1.o
		$(CC) task12_1.o -o task12_1  

2: task12_2_cli.o task12_2_serv.o
		$(CC) task12_2_cli.o -o task12_2_cli
		$(CC) task12_2_serv.o -o task12_2_serv 	

3: ncursesdirmanager.o ncursescmd.o task12_3.o		
		$(CC) ncursesdirmanager.o ncursescmd.o task12_3.o -o task12_3 $(LDFLAGS) 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS)

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)