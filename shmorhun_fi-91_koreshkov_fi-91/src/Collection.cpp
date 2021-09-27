#include "Collection.h"

Node::Node(int sz, Node* lnode, Node* rnode, Node* par, bool isuserset)
{
    parent = par;
    size_of_set = sz;
    l_child = lnode;
    r_child = rnode;
    set_of_nums = new int[sz];
    is_user_set = isuserset;
};

Node::Node() {
    parent = nullptr;
    size_of_set = 0;
    l_child = nullptr;
    r_child = nullptr;
    set_of_nums = nullptr;
    is_user_set = 0;
}

RDTree::RDTree(Node* new_root)
{
    root = new_root;
}

RDTree::RDTree() {
    root = nullptr;
}
