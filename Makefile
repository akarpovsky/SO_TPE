CC = gcc
FILES = ./src/server/server.c ./src/io/io.c ./src/utils/LinkedList.c
OUT_DIR=./run
OUT_EXE = server
#OUT_EXE = $(addprefix $(OUT_DIR)/,server)
COPTS= -Wall 
LDOPTS= -lpthread

server: $(FILES)
	$(CC) $(COPTS) -o $(OUT_EXE) $(FILES) $(LDOPTS)

clean:
	rm -f *.o core

rebuild: clean build