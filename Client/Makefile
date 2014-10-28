CC = gcc

ROOT_DIR = ..
INC = -I$(ROOT_DIR)/Lib/net -I$(ROOT_DIR)/Common

CLI_SRC = tcp_client.c chat_client.c
CLI_OBJ = tcp_client.o chat_client.o
OBJ_DIR = $(ROOT_DIR)/Obj

all: cli

cli: $(CLI_OBJ)
	@echo "Compiling client module.."
	$(CC) $(CLI_OBJ) -L../Lib/net -lnet -lpthread $(INC) -o cli -g
	@echo "Compiling client module done.."
$(CLI_OBJ): $(CLI_SRC)
	$(CC) -c $(CLI_SRC) $(INC)
install:
	cp cli $(OBJ_DIR)/
	cp cli $(ROOT_DIR)/
.PHONY: clean
clean:
	-rm *.o
	-rm cli
