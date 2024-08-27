CC := gcc
BIN := cr_serv cr_cli
OBJ := semV.o cr_serv.o ncursescr.o cr_cli.o 
CFLAGS := -g
PFLAG := -lrt
LDFLAGS := -lncurses
PEDFLAGS := -Wall -Werror

.PHONY: clean 

all: cr_serv cr_cli

# Правило для сборки cr_serv
cr_serv: cr_serv.o
		$(CC) cr_serv.o -o $@ 

# Правило для сборки cr_cli
cr_cli: semV.o ncursescr.o cr_cli.o
		$(CC) semV.o ncursescr.o cr_cli.o -o $@ $(LDFLAGS)

# Правила для создания .o файлов
%.o: %.c
		$(CC) $(CFLAGS) $< -c -o $@ $(PEDFLAGS)

clean:
		rm -rf $(BIN) $(OBJ)