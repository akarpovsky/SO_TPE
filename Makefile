CC = gcc
FILES = ./src/server/server.c ./src/IO/io.c ./src/utils/LinkedList.c ./src/server/execute.c
OUT_DIR=./run
OUT_EXE = server
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
COPTS= -Wall -g
LDOPTS= -lpthread
MATH = -lm

SOCKET_SERVER = ./src/transport/sockets/socket_s.c

socket_server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(SOCKET_SERVER) $(LDOPTS) -D sockets $(MATH)

MQ_SERVER = ./src/transport/MQ/mq_s.c

mq_server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(MQ_SERVER) $(LDOPTS) -D msgqueue $(MATH)



CLIENT_CC =  gcc
CLIENT_FILES = ./src/client/client.c ./src/client/parseCommand.c ./src/marshalling/marshalling.c ./src/utils/LinkedList.c 
#CLIENT_OUT_DIR=./run
CLIENT_OUT_EXE = client
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
CLIENT_COPTS= -Wall -g 
CLIENT_LDOPTS= -lpthread

SOCKET_CLIENT = ./src/transport/sockets/socket_c.c

socket_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(SOCKET_CLIENT) $(CLIENT_LDOPTS)

MQ_CLIENT = ./src/transport/MQ/mq_c.c

mq_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(MQ_CLIENT) $(CLIENT_LDOPTS)

clean:
	rm -f *.o core

rebuild: clean build
