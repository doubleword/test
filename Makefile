.PHONY: all server client clean

all: server client

server:
	$(MAKE) -C server
	cp server/server.elf ./



client:
	$(MAKE) -C client
	cp client/client.elf ./



clean:
	rm -rf client.elf server.elf
	$(MAKE) -C client clean
	$(MAKE) -C server clean