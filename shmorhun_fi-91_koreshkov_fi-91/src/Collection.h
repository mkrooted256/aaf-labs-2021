#pragma once
#include "Set.h"
#include <vector>

class Node
{
public:
    Node* parent;
    bool is_real = true;
    Set   set;
    Node* subnodes[2];

    //Node(Node * _parent) : parent(_parent), subnodes{}, is_real(false) { };
    Node(Node* _parent) : parent(_parent), subnodes{} { };
    //Node(Node* _parent, const Set& set) : parent(_parent), subnodes{}, set(set), is_real(true) { };
    Node(Node * _parent, const Set& set) : parent(_parent), subnodes{}, set(set) { };

    void Insert(const Set& new_set);
    void ExpandTo(const Set& to_set);
    void InsertSubset(const Set& new_set);



protected:

    unsigned int subset_test(const Set& set) const;
    unsigned int superset_test(const Set& set) const;
};

class Collection
{
    Node* root;

    void PrintSubtree(std::ostream& os, Node* node, std::string prefix, std::string) const;

public:
    Collection() : root(nullptr) {};
    void Print(std::ostream& os) const;
    void Insert(const Set& new_set);
};
