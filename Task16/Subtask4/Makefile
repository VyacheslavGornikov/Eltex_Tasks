CC := gcc
BIN := raw_cl_udp raw_cl_net raw_cl_link udp_sv_link
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := raw_cl_udp.o raw_cl_net.o raw_cl_link.o udp_sv_link.o
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2

1: raw_cl_udp.o		 
		$(CC) raw_cl_udp.o -o raw_cl_udp

2: raw_cl_net.o		 
		$(CC) raw_cl_net.o -o raw_cl_net

3: raw_cl_link.o		 
		$(CC) raw_cl_link.o -o raw_cl_link

link_sv: udp_sv_link.o
		$(CC) udp_sv_link.o -o udp_sv_link	 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS) 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)