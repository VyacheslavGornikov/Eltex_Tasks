CC := gcc
BIN := udp_sv_un udp_cl_un tcp_sv_un tcp_cl_un udp_sv_inet udp_cl_inet tcp_sv_inet tcp_cl_inet
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := udp_sv_un.o udp_cl_un.o tcp_sv_un.o tcp_cl_un.o udp_sv_inet.o udp_cl_inet.o tcp_sv_inet.o tcp_cl_inet.o
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1 2 

1: tcp_sv_un.o tcp_cl_un.o
		$(CC) tcp_sv_un.o -o tcp_sv_un 
		$(CC) tcp_cl_un.o -o tcp_cl_un 

2: udp_sv_un.o udp_cl_un.o
		$(CC) udp_sv_un.o -o udp_sv_un 
		$(CC) udp_cl_un.o -o udp_cl_un 

3: tcp_sv_inet.o tcp_cl_inet.o
		$(CC) tcp_sv_inet.o -o tcp_sv_inet 
		$(CC) tcp_cl_inet.o -o tcp_cl_inet

4: udp_sv_inet.o udp_cl_inet.o
		$(CC) udp_sv_inet.o -o udp_sv_inet 
		$(CC) udp_cl_inet.o -o udp_cl_inet		 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS) 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)