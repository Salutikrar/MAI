#ifndef _SERVER_H
#define _SERVER_H

#include "list.h"
#include "socket.h"
#include "wrap_zmq.h"

#define msg_wait_time 1

class Server{
public:
        Server();
        ~Server();
        void print_list();
        void send(Message msg);
        Message receive();
        void create_child(int id, int parent);
        void remove_child(int id);
        void exec_child(int id);
        pid_t get_pid();
        bool check(int id);
        Socket*& get_publisher();
        Socket*& get_subscriber();
        void* get_context();
        list& get_list();
        Message last_msg;
private:
        pid_t pid;
        list t;
        void *context = nullptr;
        Socket* publisher;
        Socket* subscriber;
        bool working;
        pthread_t receive_msg;
};

#endif