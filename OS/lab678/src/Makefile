all_done: server client

server: server.cpp socket.cpp wrap_zmq.cpp list.cpp
	g++ server.cpp socket.cpp wrap_zmq.cpp list.cpp -lzmq -o server -pthread -std=c++17

client: client.cpp socket.cpp wrap_zmq.cpp list.cpp
	g++ client.cpp socket.cpp wrap_zmq.cpp list.cpp -lzmq -o client -std=c++17