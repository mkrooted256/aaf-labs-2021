#include "Collection.h"
#include <stack>



void Collection::PrintSubtree(std::ostream& os, Node* subtree_root, std::string prefix, std::string children_prefix) const {
    if (subtree_root == nullptr) {
        return;
    }
    os << prefix << subtree_root->set << std::endl;

    PrintSubtree(os, subtree_root->subnodes[0], children_prefix + "L-- ", children_prefix + "|   ");
    PrintSubtree(os, subtree_root->subnodes[1], children_prefix + "L-- ", children_prefix + "    ");
}

void Collection::Print(std::ostream& os) const {
    if (!root) {
        os << "Empty Collection";
        return;
    }

    PrintSubtree(os, root, "", "");

}


Node* Collection::Insert(const Set& new_set) {
    if (!root) { 
        root = new Node(nullptr, new_set);
        root->subnodes[0] = new Node(root, new_set);
        return  root->subnodes[0];
    }
    return root->Insert(new_set);
}


Node* Node::Insert(const Set& new_set) {

    if (new_set == set) {
 
            return InsertSubset(new_set);
  
    }
    
    Set orig_set = set;
    ExpandTo(new_set);
    if (IsLeaf(this) && new_set.is_real)
    {
        InsertSubset(orig_set);
        return InsertSubset(new_set);

    }
  
    
    InsertSubset(new_set);
    return InsertSubset(orig_set);
 
}







/* Returns a bitmask of subnodes, where `set` does fit:
*    0 = 00b -  No subnode is a SUPERSET of `set`
*    1 = 01b - set <= subnode[0]
*    2 = 10b - set <= subnode[1]
*    3 = 11b - set <= subnode[0], subnode[1]
*/
unsigned int Node::superset_test(const Set& set) const {
    unsigned int mask = 0;
    if (subnodes[0] && set.IsSubsetOf(subnodes[0]->set)) {
        mask |= 1;
    }
    if (subnodes[1] && set.IsSubsetOf(subnodes[1]->set)) {
        mask |= 2;
    }
    return mask;
}

/* Returns a bitmask of subnodes with subsets of `set`:
*    0 = 00b -  No subnode is a SUBSET of `set`
*    1 = 01b - subnode[0] <= set
*    2 = 10b - subnode[1] <= set
*    3 = 11b - subnode[0], subnode[1] <= set 
*/
unsigned int Node::subset_test(const Set& set) const {
    unsigned int mask = 0;
    if (subnodes[0] && subnodes[0]->set.IsSubsetOf(set)) {
        mask |= 1;
    }
    if (subnodes[1] && subnodes[1]->set.IsSubsetOf(set)) {
        mask |= 2;
    }
    return mask;
}


/*
Returns either the node is leaf or not
*/

bool Node::IsLeaf(Node in_node) {
    if (in_node.subnodes[0] == nullptr && in_node.subnodes[1] == nullptr)
    {
        return true;
    }
    else false;
}

void Node::ExpandTo(const Set& to_set) {
    set = set.Union(to_set);
}

