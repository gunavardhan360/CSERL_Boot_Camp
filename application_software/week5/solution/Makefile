
CC = g++ -ggdb3 -Wno-write-strings
CLIENT_FILE = echo_client.cpp
SERVER_FILE = get_server.cpp
HTTP_SERVER_FILE = http_server.cpp

all: server client

server: $(SERVER_FILE) $(HTTP_SERVER_FILE)
	$(CC) $(SERVER_FILE) $(HTTP_SERVER_FILE) -o server

client: $(CLIENT_FILE)
	$(CC) $(CLIENT_FILE) -o client

clean:
	rm -f server client
