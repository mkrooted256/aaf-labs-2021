#include "Collection.h"
#include <stack>

Set UniSets(Set f_set, Set s_set) {
    return f_set.Union(s_set);
}

bool IsLeaf(Node* node) {
    if (node->subnodes[0] == nullptr && node->subnodes[1] == nullptr)
    {
        return true;
    }
    return false;
}




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


void Collection::Insert(const Set& new_set) {
    if (!root) { 
        root = new Node(nullptr, new_set);
        root->is_real = false;
        root->subnodes[0] = new Node(root, new_set);
        return;
    }
    root->ExpandTo(new_set);
    return root->Insert(new_set);
}

void Node::Insert(const Set& new_set) {
    // Step 0: check if Node is empty.

    if (subnodes[0] == nullptr) {
        // No subnodes at all. Just add the first subnode.
        subnodes[0] = new Node(this, new_set);
        subnodes[1] = new Node(this,set);
        ExpandTo(new_set);
        return;
    }

    if (subnodes[1] == nullptr) {
        // just insert as new subnode
        subnodes[1] = new Node(this, new_set);
        return;
    }
    this->ExpandTo(new_set);
    // Step 1: find MIN{new_set \ subnode[i]}
    //    It will show, how much do we need to expand a node to fit the set in it

    short argmin_i;
    Set complements[2] = {
        new_set.Minus(subnodes[0]->set),
        (subnodes[1] != nullptr) ? new_set.Minus(subnodes[1]->set) : Set(),
    };
    size_t complement_sizes[2] = {
        complements[0].Size(),
        complements[1].Size()
    };


    // Step 2: Subset test
    //     because (|X\A| = 0) => (X is a subset of A)

    if (complement_sizes[0] == 0) {
        subnodes[0]->Insert(new_set);
        return;
    }

    if (complement_sizes[1] == 0) {
        // set <= subnode[1]
        subnodes[1]->Insert(new_set);
        return;
    }
    
    // Step 3: General insert

    if (complement_sizes[0] <= complement_sizes[1]) {
        argmin_i = 0;
    }
    else {
        argmin_i = 1;
    }
    subnodes[argmin_i]->Insert(new_set);
    return;
}

    /* Returns a bitmask of subnodes, where `set` does fit:
    *    0 = 00b -  No subnode is a SUPERSET of `set`
    *    1 = 01b - set <= subnode[0]
    *    2 = 10b - set <= subnode[1]
    *    3 = 11b - set <= subnode[0], subnode[1]
    */
    unsigned int Node::superset_test(const Set & set) const {
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

void Node::ExpandTo(const Set& to_set) {
    set = set.Union(to_set);
}



