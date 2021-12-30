#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <vector>
#include <utility>
#include <algorithm>
#include <sstream>
using namespace std;

vector<string> v_namer(0);
vector<string> games(0);
vector<pair<string,string>> invites(0);
int k = 0;


void n_reader() {
        while (true) {
                ifstream fin("namer");
                char buff[256];
                while (fin.getline(buff, 50, '\n')) {
                        string str = string(buff);
                        string identificator, action, third;
                        istringstream iss(str);
                        getline(iss, identificator, ' ');
                        getline(iss, action, ' ');
                        getline(iss, third, ' ');
                        if (action.length() == 0) {
                                v_namer.push_back(str);
                                cout << v_namer[k] << '\n';
                                k++;
                                continue;
                        }
                        else if (action == "create") {
                                cout << str << '\n';
                                ofstream request1(identificator);
                                request1 << "true" << '\n';
                                request1.close();
                                games.push_back(identificator);
                                continue;
                        }
                        else if (action == "invite") {
                                cout << str << '\n';
                                if (find(v_namer.begin(), v_namer.end(), third) != v_namer.end()) {
                                        ofstream request2(identificator);
                                        request2 << "true" << '\n';
                                        request2.close();
                                        invites.push_back(make_pair(identificator, third));
                                        continue;
                                }
                                else {
                                        ofstream request3(identificator);
                                        request3 << "false" << '\n';
                                        request3.close();
                                        continue;
                                }
                        }
                        else if (action == "join") {
                                cout << str << '\n';
                                int typ = 0;
                                for(int iterator = 0; iterator < invites.size(); iterator++){
                                        if (invites[iterator].second == identificator) {
                                                ofstream request4(identificator);
                                                request4 << invites[iterator].first << '\n';
                                                invites.erase(invites.begin() + iterator);
                                                request4.close();
                                                typ = 1;
                                                break;
                                        }
                                }
                                if (typ == 0) {
                                        ofstream request5(identificator);
                                        request5 << "false" << '\n';
                                        request5.close();
                                }
                        }
                }
                fin.close();
        }
}

int main(int argc, char* argv[]){
        if ((mkfifo("namer", 0644)) == -1){
                fprintf(stderr, "Невозможно создать fifo\n");
                exit(0);
        }
        thread name_thread(n_reader);

        name_thread.join();
}
