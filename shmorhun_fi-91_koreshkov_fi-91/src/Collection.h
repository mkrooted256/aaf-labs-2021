#pragma once
#include "Set.h"
#include <vector>

class Node
{
public:
    Node* parent;

    Set   set;
    bool  is_real;
    Node* subnodes[2];

    Node(Node * _parent) : parent(_parent), subnodes{}, is_real(false) { };
    Node(Node * _parent, const Set& set) : parent(_parent), subnodes{}, set(set), is_real(true) { };
    
    Node * Insert(const Set& new_set);
    void ExpandTo(const Set& to_set);
    Node* InsertSubset(const Set& new_set);

protected:

    unsigned int subset_test(const Set& set) const;
    unsigned int superset_test(const Set& set) const;
};

class Collection
{
    Node* root;
public:
    Collection() : root(nullptr) {};
    void Print(std::ostream& os) const;
    Node* Insert(const Set& new_set);
};
