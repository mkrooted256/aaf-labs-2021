#pragma once
#include "Set.h"

class Node
{
public:
    static const size_t DEFAULT_NODE_ARR_SIZE = 3;
    std::vector<std::pair<Set, Node*>> sets_pointers;
    Node* parent = nullptr;
    
    Node() {
        sets_pointers = std::vector<std::pair<Set, Node*>> (DEFAULT_NODE_ARR_SIZE);
    };
    
    void Insert(Set& set);
    
    int IndexOfChildVector(std::vector<std::pair<Set, Node*>>& node_vec, Node* node);
    Set UnionNodeSets(std::vector<std::pair<Set, Node*>>& vector_to_union);
    std::pair<int, int> HaveEmptySpace(std::vector<std::pair<Set, Node*>> vector_to_check);
    std::pair<int,int> GetMinimumSetSizeAndIndex(std::vector<std::pair<Set, Node*>>& set_vec);
    std::pair<int, int> ShortestSubTree(std::vector<std::pair<Set, Node*>>);
};

class Tree : Node
{
public:
    
    Node root;
    Tree() { root = Node(); };


};
