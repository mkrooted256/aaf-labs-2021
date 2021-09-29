#pragma once
#include "Set.h"

class Node
{
public:
    Set set = Set();
    std::vector<Node*> children(const static int size_chld_array = 10);
    Node* parent = nullptr;
    bool is_user_set = 0;
    
    Node() {};
    
    Node(Set set_to_in, Node* choose_parent, bool is_it_users) {
        this->set = set_to_in;
        this->parent = choose_parent;
        this->is_user_set = is_it_users;
    }
};

class Tree
{
public:
    
    Node* root;

    Tree(Set first_set) {
        root = new Node(first_set, nullptr, 1);
        Insert(root->set);
    };

    Tree() { root = new Node(); };
    void Insert(Set set_to_insert);
};
