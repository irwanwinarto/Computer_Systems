all: pa4_server pa4_client

pa4_server: server.cpp
	g++ server.cpp -o pa4_server -lpthread -std=c++11

pa4_client: client.cpp
	g++ client.cpp -o pa4_client -std=c++11

clean:
	rm -f pa4_server
	rm -f pa4_client