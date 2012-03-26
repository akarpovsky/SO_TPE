CC = gcc
FILES = ./src/server/server.c ./src/IO/io.c ./src/utils/LinkedList.c
OUT_DIR=./run
OUT_EXE = server
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
COPTS= -Wall -g
LDOPTS= -lpthread

server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(LDOPTS)


CLIENT_CC =  gcc
CLIENT_FILES = ./src/client/client.c ./src/transport/fifo_c.c ./src/marshalling/marshalling.c 
#CLIENT_OUT_DIR=./run
CLIENT_OUT_EXE = server
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
CLIENT_COPTS= -Wall -g 
CLIENT_LDOPTS= -lpthread

client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(CLIENT_LDOPTS)

clean:
	rm -f *.o core

rebuild: clean build
