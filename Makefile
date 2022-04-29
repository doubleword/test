.PHONY: all server client clean testfile

all: server client

server:
	$(MAKE) -C server
	cp server/server.elf ./



client:
	$(MAKE) -C client
	cp client/client.elf ./


testfile:
	dd if=/dev/urandom of=./testfile bs=1024 count=4

clean:
	rm -rf client.elf server.elf testfile
	$(MAKE) -C client clean
	$(MAKE) -C server clean