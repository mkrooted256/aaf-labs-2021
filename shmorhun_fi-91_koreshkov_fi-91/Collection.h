#pragma once

class Node
{
public:
    int size_of_set;
    int* set_of_nums;
    Node* l_child;
    Node* r_child;
    Node* parent;
    bool is_user_set;
    Node(int sz, Node* lnode, Node* rnode, Node* prnt, bool isuserset);
    Node();
};

class RDTree
{
public:
    Node* root;
    RDTree();
    RDTree(Node* new_root);

private:

};