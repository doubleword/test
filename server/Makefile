
CC = gcc

SRC = main.c connection.c clargs.c
HDR = connection.h clargs.h

server.elf: $(SRC) $(HDR)
	$(CC) $(SRC) -pthread -o server.elf


.PHONY:
clean:
	rm -rf server.elf


