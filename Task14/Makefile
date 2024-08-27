CC := gcc
BIN := posix_serv posix_cli sysV_serv sysV_cli 
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := posix_serv.o posix_cli.o sysV_serv.o sysV_cli.o
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 

1: posix_serv.o posix_cli.o
		$(CC) posix_serv.o -o posix_serv $(PFLAG)
		$(CC) posix_cli.o -o posix_cli $(PFLAG)

2: sysV_serv.o sysV_cli.o
		$(CC) sysV_serv.o -o sysV_serv 
		$(CC) sysV_cli.o -o sysV_cli  

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)