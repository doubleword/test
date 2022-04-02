SERVER_SRC = server/main.c server/clargs.c server/connection.c
SERVER_HEADERS = server/clargs.h

CLIENT_SRC = client/main.c client/clargs.c 
CLIENT_HEADERS = client/clargs.h

CC = gcc


.PHONY: all
all: server.elf client.elf

server.elf: $(SERVER_SRC) $(SERVER_HEADERS)
	$(CC) $(SERVER_SRC) -pthread -o server.elf

client.elf: $(CLIENT_SRC) $(CLIENT_HEADERS)
	$(CC) $(CLIENT_SRC) -o client.elf

testfile:
	dd if=/dev/urandom of=./testfile bs=1024 count=4

.PHONY: clean
clean:
	rm -rf client.elf server.elf testfile