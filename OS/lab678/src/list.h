#ifndef _LIST_H
#define _LIST_H

#include <vector>
using namespace std;

class list_el{
        list_el* next;
        int value;
public:
        list_el(int new_val);
        int& get_value();
        list_el*& get_next();
};

class list {
private:
        list_el* root;
        void delete_list(list_el*& cur);
        static void print_list(list_el*& cur);
        bool find_el(list_el* cur, int val);
        bool find_el_in_end(list_el* cur, int val);
        void insert_el(list_el*& cur, int new_val);
        void delete_el_list(list_el*& cur, int val);
        int get_parent(list_el*& cur);
public:
        list();
        list(int new_val);
        void insert(int new_val);
        bool find(int val);
        bool find_in_end(int val);
        void print();
        void delete_el(int val);
        int get_place();
        void get_all(list_el* cur, vector<int>& tmp);
        vector<int> get_all_elems();
        ~list();
};

#endif