#ifndef _CLIENT_H
#define _CLIENT_H

#include <string.h>
#include <iostream>
#include <unistd.h>
#include "wrap_zmq.h"
#include "socket.h"
#include <map>
using namespace std;

class Client{
private:
        int id;
        void* context;
        bool terminated;
        map<string,int> map1;
public:
        Socket* child_publisher_next;
        Socket* parent_publisher;
        Socket* parent_subscriber;
        Socket* next_subscriber;
        Client(int new_id, string parent_endpoint, int new_parent_id, map<string,int> &new_map);
        ~Client();
        bool& get_status();
        void send_up(Message msg);
        void send_down(Message msg);
        Message receive();
        map<string,int> &get_map();
        int get_id();
        int add_child(int id);
        int parent_id;
};

#endif