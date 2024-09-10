CC := gcc
BIN := raw_cl_udp 
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := raw_cl_udp.o 
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 

1: raw_cl_udp.o		 
		$(CC) raw_cl_udp.o -o raw_cl_udp	 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS) 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)