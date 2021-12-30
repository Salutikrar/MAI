#include <iostream>
#include <vector>
#include "list.h"
using namespace std;

list_el::list_el(int new_val) : value(new_val), next(nullptr){}

int& list_el::get_value(){
        return value;
}

list_el*& list_el::get_next(){
        return next;
}






void list::delete_list(list_el*& cur){
        if(!cur) return;
        delete_list(cur->get_next());
        delete cur;
}

void list::print_list(list_el*& cur){
        if(!cur) {
                cout <<endl;
                return;
        }
        cout << cur->get_value() << " ";
        print_list(cur->get_next());        
}

bool list::find_el(list_el* cur, int val){
        if(cur == nullptr){
                return false;
        }
        if(val != cur->get_value()){
                return find_el(cur->get_next(), val);
        } else {
                return true;
        }
}

bool list::find_el_in_end(list_el* cur, int val){
        if(cur == nullptr){
                return false;
        }
        if((val == cur->get_value()) && (cur->get_next() == nullptr)){
                return true;
        } else {
                return find_el(cur->get_next(), val);
        }
}

void list::insert_el(list_el*& cur, int new_val){
        if(cur == nullptr){
                cur = new list_el(new_val);
        } else {
                return insert_el(cur->get_next(), new_val);
        }
}

void list::delete_el_list(list_el*& cur, int val){
        if(!cur) return;
        if(val != cur->get_value()){
                delete_el_list(cur->get_next(), val);
        } else{
                delete_list(cur);
                cur = nullptr;
        }
}

int list::get_parent(list_el*& cur){
        if(cur == nullptr){
                return -1;
        }
        if(cur->get_next() == nullptr){
                return cur->get_value();
        } else {
                return get_parent(cur->get_next());
        }
}





list::list():root(nullptr){}

list::list(int new_val){
        root = new list_el(new_val);
}

void list::insert(int new_val){
        insert_el(root, new_val);
}

bool list::find(int val){
        return find_el(root, val);
}

bool list::find_in_end(int val){
        return find_el_in_end(root, val);
}

void list::print(){
        print_list(root);
}

void list::delete_el(int val){
        delete_el_list(root, val);
}

int list::get_place(){
        return get_parent(root);
}

void list::get_all(list_el* cur, vector<int>& tmp){
        if(!cur) return;
        tmp.push_back(cur->get_value());
        get_all(cur->get_next(), tmp);
}

vector<int> list::get_all_elems(){
        vector<int> tmp;
        get_all(root, tmp);
        return tmp;

}

list::~list() {
        delete_list(root);
}

