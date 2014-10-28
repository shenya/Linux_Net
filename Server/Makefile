CC = gcc

ROOT_DIR = ..
INC = -I$(ROOT_DIR)/Lib/net -I$(ROOT_DIR)/Common
OBJ_DIR = $(ROOT_DIR)/Obj
SERV_SRC = tcp_server.c chat_server.c
SERV_OBJ = tcp_server.o chat_server.o

all: serv

serv: $(SERV_OBJ)
	@echo "Compiling server module.."
	$(CC) $(SERV_OBJ) -L../Lib/net -lnet -lpthread $(INC) -o serv -g
	@echo "Compiling client module done.."
$(SERV_OBJ): $(SERV_SRC)
	$(CC) -c $(SERV_SRC) $(INC)

install:
	cp serv $(OBJ_DIR)/
	cp serv $(ROOT_DIR)/
.PHONY: clean
clean:
	-rm *.o
	-rm serv
