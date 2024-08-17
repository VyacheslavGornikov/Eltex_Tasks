CC := gcc
BIN := posix_serv posix_cli sysV_serv sysV_cli cr_serv cr_cli
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := posix_serv.o posix_cli.o sysV_serv.o sysV_cli.o cr_serv.o cr_cli.o 
CFLAGS := -g
PFLAG := -lrt
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 3

1: posix_serv.o posix_cli.o
		$(CC) posix_serv.o -o posix_serv $(PFLAG)
		$(CC) posix_cli.o -o posix_cli $(PFLAG)

2: sysV_serv.o sysV_cli.o
		$(CC) sysV_serv.o -o sysV_serv 
		$(CC) sysV_cli.o -o sysV_cli  	

3: cr_serv.o cr_cli.o 		
		$(CC) cr_serv.o -o cr_serv $(LDFLAGS)
		$(CC) cr_cli.o -o cr_cli $(LDFLAGS) 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)