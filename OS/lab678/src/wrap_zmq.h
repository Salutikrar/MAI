#ifndef _WRAP_ZMQ_H
#define _WRAP_ZMQ_H

#include <tuple>
#include <vector>
#include <atomic>
#include <zmq.h>
#include "wrap_zmq.h"
using namespace std;

#define UNIVERSAL_MSG -1
#define SERVER_ID -2
#define PARENT_SIGNAL -3

enum struct SocketType{
        PUBLISHER,
        SUBSCRIBER,
};

enum struct CommandType {
        ERROR,
        RETURN,
        CREATE_CHILD,
        REMOVE_CHILD,
        EXEC_CHILD,
};

enum struct EndpointType{
        CHILD_PUB_NEXT,
        PARENT_PUB,
};

void* create_zmq_ctx();
void destroy_zmq_ctx(void* context);
int get_zmq_socket_type(SocketType type);
void* create_zmq_socket(void* context, SocketType type);
void close_zmq_socket(void* socket);
string create_endpoint(EndpointType type, pid_t id);
void bind_zmq_socket(void* socket, string endpoint);
void unbind_zmq_socket(void* socket, string endpoint);
void connect_zmq_socket(void* socket, string endpoint);
void disconnect_zmq_socket(void* socket, string endpoint);

#define MAX_CAP 1000

class Message {
protected:
        static std::atomic<int> counter;
public:
        CommandType command = CommandType::ERROR;
        int to_id;
        int create_id;
        int uniq_num;
        bool to_up;
        int k=0;
        char name[MAX_CAP] = {' '};
        bool change=false;
        int val=0;
        bool error=false;
        Message();
        Message(CommandType new_command, int new_to_id, int new_k, char* new_name, bool new_change, int new_val, bool new_error, int new_id);
        Message(CommandType new_command, int new_to_id, int new_id);
        friend bool operator==(const Message& lhs, const Message& rhs);
        int& get_create_id();
        int& get_to_id();
};

void create_zmq_msg(zmq_msg_t* zmq_msg, Message& msg);
void send_zmq_msg(void* socket, Message& msg);
Message get_zmq_msg(void* socket);

#endif