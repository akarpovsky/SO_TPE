CC = gcc
FILES = ./src/server/server.c ./src/IO/io.c ./src/utils/hashmap.c ./src/utils/LinkedList.c ./src/utils/printColor.c ./src/server/execute.c
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

FIFO_SERVER = ./src/transport/fifos/fifo_s.c ./src/transport/fifos/fifo_contact_serialization.c ./src/marshalling/message_serialization.c

fifo_server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(FIFO_SERVER) $(LDOPTS) -D fifo $(MATH)

SHMM_SERVER = ./src/transport/shmm/shmm_s.c ./src/transport/shmm/shmm_contact_serialization.c ./src/marshalling/message_serialization.c

shmm_server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(SHMM_SERVER) $(LDOPTS) -D shmm $(MATH) -lrt

CLIENT_CC =  gcc
CLIENT_FILES = ./src/client/client.c ./src/client/parseCommand.c ./src/utils/hashmap.c ./src/marshalling/marshalling.c ./src/utils/LinkedList.c
#CLIENT_OUT_DIR=./run
CLIENT_OUT_EXE = client
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
CLIENT_COPTS= -g #-Wall -g 
CLIENT_LDOPTS= -lpthread

SOCKET_CLIENT = ./src/transport/sockets/socket_c.c

socket_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(SOCKET_CLIENT) -D sockets $(CLIENT_LDOPTS)

MQ_CLIENT = ./src/transport/MQ/mq_c.c

mq_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(MQ_CLIENT) -D msgqueue $(CLIENT_LDOPTS)

FIFO_CLIENT = ./src/transport/fifos/fifo_c.c ./src/transport/fifos/fifo_contact_serialization.c ./src/marshalling/message_serialization.c

fifo_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(FIFO_CLIENT) -D fifo $(CLIENT_LDOPTS)

SHMM_CLIENT = ./src/transport/shmm/shmm_c.c ./src/transport/shmm/shmm_contact_serialization.c ./src/marshalling/message_serialization.c

shmm_client: $(CLIENT_FILES)
	$(CLIENT_CC) $(CLIENT_COPTS) -o $(CLIENT_OUT_EXE) $(CLIENT_FILES) $(SHMM_CLIENT) -D shmm $(CLIENT_LDOPTS) -lrt

clean:
	rm -f *.o core

rebuild: clean build
