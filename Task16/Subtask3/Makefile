CC := gcc
BIN := udp_sv_broadcast udp_cl_broadcast udp_sv_multicast udp_cl_multicast
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := udp_sv_broadcast.o udp_cl_broadcast.o udp_sv_multicast.o udp_cl_multicast.o
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 

1: udp_sv_broadcast.o udp_cl_broadcast.o
		$(CC) udp_sv_broadcast.o -o udp_sv_broadcast 
		$(CC) udp_cl_broadcast.o -o udp_cl_broadcast		 

2: udp_sv_multicast.o udp_cl_multicast.o
		$(CC) udp_sv_multicast.o -o udp_sv_multicast 
		$(CC) udp_cl_multicast.o -o udp_cl_multicast

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS) 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)