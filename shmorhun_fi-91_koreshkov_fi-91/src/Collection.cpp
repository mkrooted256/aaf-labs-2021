#include "Collection.h"
#include <stack>


void Collection::Print(std::ostream& os) const {
    if (!root) {
        os << "Empty Collection";
        return;
    }

    std::stack<Node*> nodes;
    std::stack<unsigned int> lvls;

    nodes.push(root->subnodes[1]);
    lvls.push(1);
    nodes.push(root->subnodes[0]);
    lvls.push(1);


    os << root->set;
    if (root->set.is_real) os << "*";
    os << std::endl;

    while (!nodes.empty()) {
        Node* next = nodes.top();
        nodes.pop();
        unsigned int depth = lvls.top();
        lvls.pop();

        if (next != nullptr) {
            // Indent
            for (unsigned int i = 0; i < depth - 1; i++) {
                os << "|   ";
            }
            os << "+-- " << next->set;
            if (next->set.is_real) os << "*";
            os << std::endl;

            nodes.push(next->subnodes[1]);
            lvls.push(depth + 1);
            nodes.push(next->subnodes[0]);
            lvls.push(depth + 1);
        }
    }
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

