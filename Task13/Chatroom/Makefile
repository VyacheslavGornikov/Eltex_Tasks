CC := gcc
BIN := cr_serv cr_cli
SERV_OBJ := msgserver.o cr_serv.o
CLI_OBJ := msgclient.o ncursescr.o cr_cli.o
CFLAGS := -g
PFLAG := -lrt
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean 

all: cr_serv cr_cli

# Правило для сборки cr_serv
cr_serv: msgserver.o cr_serv.o
		$(CC) msgserver.o cr_serv.o -o $@ 

# Правило для сборки cr_cli
cr_cli: msgclient.o ncursescr.o cr_cli.o
		$(CC) msgclient.o ncursescr.o cr_cli.o -o $@ $(LDFLAGS)

# Правила для создания .o файлов
%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS)

clean:
		rm -rf $(BIN) $(SERV_OBJ) $(CLI_OBJ)