# Тестовое задание
**Клиент** позволяет отправить указанный файл на сервер. Также можно указать с каким именем создать файл на сревере.

**Сервер** принимает файл от клиента и сохраняет его на диск. Если файл с данным именем уже существует, то он перезаписывается. Способен принимать несколько файлов параллельно.    

# Compile
```
git clone https://github.com/doubleword/vk2022internshiptest.git
cd test
make
```

# Server
```
Usage: ./server.elf [OPTIONS]

Options:
-h               Print help and exit
-p PORT          Port to listen. Default: 8080
-d DIR           Directory for saving files. Default: ./
```
# Client
```
Usage: ./client.elf [OPTIONS] [--] IP PORT FILEPATH

Options:
-h               Print help and exit
-n FILENAME      The name of the file under which it will be created on the server

Positional arguments:
IP               Server IP
PORT             Port to connect to
FILEPATH         Path of the file to send
```

# Example
**Generate test file:**
```
make testfile 
```
**Server:**
```
./server.elf -p 8888 -d /tmp
```

**Client:**
```
./client.elf 0.0.0.0 8888 ./testfile

./client.elf 0.0.0.0 8888 ./testfile -n new.filename
```

