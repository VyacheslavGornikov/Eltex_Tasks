CC := gcc
BIN := simple_par_sv par_client pool_par_sv tcp_cl_inet udp_cl_inet multi_prot_sv producer_consumer_sv
SRC := $(ls | grep *.c)
HDR := $(ls | grep *.h)
OBJ := simple_par_sv.o inet_sockets.o par_client.o pool_par_sv.o tcp_cl_inet.o udp_cl_inet.o multi_prot_sv.o producer_consumer_sv.o
CFLAGS := -g
PFLAG := -lrt -pthread
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean

all: 1

1: simple_par_sv.o inet_sockets.o par_client.o
		$(CC) inet_sockets.o simple_par_sv.o -o simple_par_sv 
		$(CC) inet_sockets.o par_client.o -o par_client

2: pool_par_sv.o inet_sockets.o par_client.o
		$(CC) inet_sockets.o pool_par_sv.o -o pool_par_sv 
		$(CC) inet_sockets.o par_client.o -o par_client

3: producer_consumer_sv.o inet_sockets.o par_client.o
		$(CC) inet_sockets.o producer_consumer_sv.o -o producer_consumer_sv 
		$(CC) inet_sockets.o par_client.o -o par_client

4: inet_sockets.o tcp_cl_inet.o udp_cl_inet.o multi_prot_sv.o
		$(CC) tcp_cl_inet.o -o tcp_cl_inet
		$(CC) udp_cl_inet.o -o udp_cl_inet
		$(CC) inet_sockets.o multi_prot_sv.o -o multi_prot_sv			 

%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS) 

$(BIN): $(HDR)

clean:
		rm -rf $(BIN) $(OBJ)