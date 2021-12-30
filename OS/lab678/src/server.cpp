#include <string.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <csignal>
#include <map>
#include "server.h"
#include "socket.h"
#include "wrap_zmq.h"

void* subscriber_thread(void* server){
        Server* server_ptr = (Server*) server;
        pid_t serv_pid = server_ptr->get_pid();
        try{
                pid_t child_pid = fork();
                if(child_pid == -1) throw runtime_error("Can not fork.");
                if(child_pid == 0){
                        execl("client", "client", "0", server_ptr->get_publisher()->get_endpoint().data(), "-1", nullptr);
                        throw runtime_error("Can not execl");
                        server_ptr->~Server();
                        return (void*)-1;
                }
                string endpoint = create_endpoint(EndpointType::PARENT_PUB, child_pid);
                server_ptr->get_subscriber() = new Socket(server_ptr->get_context(), SocketType::SUBSCRIBER, endpoint);
                for(;;){
                        Message msg = server_ptr->get_subscriber()->receive();
                        server_ptr->last_msg = msg;
                        switch(msg.command){
                                case CommandType::CREATE_CHILD:
                                        cout << "OK:" << msg.get_create_id() << endl;
                                        break;
                                case CommandType::REMOVE_CHILD:
                                        cout << "OK" << endl;
                                        server_ptr->get_list().delete_el(msg.get_create_id());
                                        break;
                                case CommandType::RETURN:
                                        break;
                                case CommandType::EXEC_CHILD:
                                        if(msg.error){
                                                cout << "OK:" << msg.get_create_id() << ": '" << msg.name << "' not found\n";
                                        } else if(msg.change){
                                                cout << "OK:" << msg.get_create_id() << endl;
                                        } else{
                                                cout << "OK:" << msg.get_create_id() << ": " << msg.val << "\n";
                                        }
                                        break;
                                default:
                                        break;
                        }
                }
        } catch(runtime_error& err){
                cout << "Server wasn't started " << err.what() << endl;
        }
        return nullptr;
}

Server::Server(){
        context = create_zmq_ctx();
        pid = getpid();
        string endpoint = create_endpoint(EndpointType::CHILD_PUB_NEXT, getpid());
        publisher = new Socket(context, SocketType::PUBLISHER, endpoint);
        if(pthread_create(&receive_msg, 0, subscriber_thread, this) != 0){
                throw runtime_error("Can not run second thread.");
        }
        working = true;
}

Server::~Server(){
        if(!working) return;
        working = false;
        send(Message(CommandType::REMOVE_CHILD, 0, 0));
        try{
                delete publisher;
                delete subscriber;
                publisher = nullptr;
                subscriber = nullptr;

                destroy_zmq_ctx(context);
                sleep(2);
        } catch (runtime_error &err){
                cout << "Server wasn't stopped " << err.what() << endl;
        }
}

pid_t Server::get_pid(){
        return pid;
}

void Server::print_list(){
        t.print();
}

bool Server::check(int id){
        Message msg(CommandType::RETURN, id, 0);
        send(msg);
        sleep(msg_wait_time);
        msg.get_to_id() = SERVER_ID;
        return last_msg == msg;
}

void Server::send(Message msg){
        msg.to_up = false;
        publisher->send(msg);
}

Message Server::receive(){
        return subscriber->receive();
}

Socket*& Server::get_publisher(){
        return publisher;
}

Socket*& Server::get_subscriber(){
        return subscriber;
}

void* Server::get_context(){
        return context;
}

list& Server::get_list(){
        return t;
}

void Server::create_child(int id,int parent){
        if(t.find(id)){
                throw runtime_error("Error:" + to_string(id) + ":Node with that number already exists.");
        }
        if(parent!=0 && !t.find_in_end(parent)){
                throw runtime_error("Error:" + to_string(id) + ":I can not do it parent or it does not exist.");
        }
        if(t.get_place() && !check(t.get_place())){ 
                throw runtime_error("Error:" + to_string(id) + ":Parent node is unavailable.");
        }
        send(Message(CommandType::CREATE_CHILD, t.get_place(), id));
        t.insert(id);
}

void Server::remove_child(int id){
        if(!t.find(id)){
                throw runtime_error("Error:" + to_string(id) + ":Node with that number doesn't exist.");
        }
        if(!check(id)){
                throw runtime_error("Error:" + to_string(id) + ":Node is unavailable.");
        }
        send(Message(CommandType::REMOVE_CHILD, id, 0));
}

#define MAX_CAP 1000

void Server::exec_child(int id){
        char name[MAX_CAP];
        char p;
        cin >> p;
        int k=0;
        while(p!=' ' && p!='\n'){
                name[k]=p;
                p=getchar();
                k++;
        }
        name[k]='\0';
        k++;
        int val=0;
        bool change=false;
        if(p==' '){
                cin >> val;
                change=true;
        }
        if(!t.find(id)){
                throw runtime_error("Error:" + to_string(id) + ":Node with that number doesn't exist.");
        }
        if(!check(id)){
                throw runtime_error("Error:" + to_string(id) + ":Node is unavailable.");
        }
        send(Message(CommandType::EXEC_CHILD, id, k, name, change, val, false, 0));
}

void process_cmd(Server& server, string cmd){
        if(cmd == "create"){
                int id;
                cin >> id;
                int parent;
                cin >> parent;
                server.create_child(id,parent);
        } else if (cmd == "remove"){
                int id;
                cin >> id;
                server.remove_child(id);
        } else if (cmd == "exec"){
                int id;
                cin >> id;
                server.exec_child(id);
        } else if(cmd == "exit"){
                throw invalid_argument("Exiting...");
        } else if(cmd == "pingall"){
                vector<int> tmp = server.get_list().get_all_elems();
                cout << "OK:";
                bool fnd = false;
                for(int& i : tmp){
                        if(!server.check(i)){
                                if(fnd) cout << ';';
                                cout << i;
                                fnd = true;
                        }
                }
                if(!fnd) cout << "-1";
                cout << endl;
        } else if(cmd == "status"){
                int id;
                cin >> id;
                if(!server.get_list().find(id)){
                        throw runtime_error("Error:" + to_string(id) + ":Node with that number doesn't exist.");
                }
                if(server.check(id)){
                        cout << "OK" << endl;
                } else{
                        cout << "Node is unavailable" << endl;
                }
        } else if(cmd == "print"){
                server.print_list();
        } else {
                cout << "It is not a command!\n";
        }
}

Server* server_ptr = nullptr;
void TerminateByUser(int) {
        if (server_ptr != nullptr) {
                server_ptr->~Server();
        }
        cout << to_string(getpid()) + " Terminated by user" << endl;
        exit(0);
}

int main (int argc, char const *argv[])
{
        try{
                if (signal(SIGINT, TerminateByUser) == SIG_ERR) {
                        throw runtime_error("Can not set SIGINT signal");
                }
                if (signal(SIGSEGV, TerminateByUser) == SIG_ERR) {
                        throw runtime_error("Can not set SIGSEGV signal");
                }
                if (signal(SIGTERM, TerminateByUser) == SIG_ERR) {
                        throw runtime_error("Can not set SIGTERM signal");
                }
                Server server;
                server_ptr = &server;
                cout << getpid() << " server started correctly!\n";
                for(;;){
                        try{
                                string cmd;
                                while(cin >> cmd){
                                        process_cmd(server, cmd);
                                }
                        } catch(const runtime_error& arg){
                                cout << arg.what() << endl;
                        }
                }
        } catch(const runtime_error& arg){
                cout << arg.what() << endl;
        } catch(...){}
        sleep(5);
        return 0;
}