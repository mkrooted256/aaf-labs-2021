#include "Collection.h"

void Node::ExpandTo(const Set& to_set) {
    set = set.Union(to_set);
}

Node* Node::Insert(const Set& new_set) {
    ExpandTo(new_set);
    return InsertSubset(new_set);
}

Node* Node::InsertSubset(const Set& new_set) {   
    /* 
    Check if new_set == set;
        We know that [new_set <= set]. 
        So [|new_set| == |set|] implies [new_set == set] 
    */
    if (new_set.Size() == set.Size()) {
        is_real = true;
        return this;
    }


    // Step 0: check if Node is empty.

    if (subnodes[0] == nullptr) {
        // No subnodes at all. Just add the first subnode.
        subnodes[0] = new Node(this, new_set);
        return subnodes[0];
    }

    // Step 1: find MIN{new_set \ subnode[i]}
    //    It will show, how much do we need to expand a node to fit the set in it

    short argmin_i;
    Set complements[2] = {
        new_set.Minus(subnodes[0]->set),
        ( subnodes[1] != nullptr ) ? new_set.Minus(subnodes[1]->set) : Set(), 
    };
    size_t complement_sizes[2] = {
        complements[0].Size(),
        complements[1].Size()
    };

    // Step 2: Subset test
    //     because (|X\A| = 0) => (X is a subset of A)

    if (complement_sizes[0] == 0) {
        if (subnodes[1] == nullptr) {
            // just insert as new subnode
            subnodes[1] = new Node(this, new_set);
            return subnodes[1];
        }
        return subnodes[0]->Insert(new_set);
    }
    if (subnodes[1] == nullptr) {
        // just insert as new subnode
        subnodes[1] = new Node(this, new_set);
        return subnodes[1];
    }
    if (complement_sizes[1] == 0) {
        // set <= subnode[1]
        return subnodes[1]->Insert(new_set);
    }

    // Step 3: Superset test

    unsigned int subset_nodes = subset_test(new_set);

    if (subset_nodes == 3) {
        // subnode[0], subnode[1] <= set

        /*  
        FROM

        parent 
        +-- this(set) 
            +-- node[1]
            +-- node[2]  
        +-- other

        TO

        parent 
        +-- this(set) 
            +-- new_node(new_set) 
                +-- node[1]
                +-- node[2]  
            +-- nullptr
        +-- other
        */
        
        Node* new_node = new Node(this, new_set);
        new_node->subnodes[0] = subnodes[0];
        new_node->subnodes[1] = subnodes[1];
        subnodes[0]->parent = new_node;
        subnodes[1]->parent = new_node;
        subnodes[0] = new_node;
        subnodes[1] = nullptr;
        return new_node;
    }

    if (subset_nodes == 1) {
        // subnode[0] <= set
        // same as in previous `if`, but only for subnode[0]

        Node* new_node = new Node(this, new_set);
        new_node->subnodes[0] = subnodes[0];
        subnodes[0]->parent = new_node;
        subnodes[0] = new_node;
        return new_node;
    }

    if (subset_nodes == 2) {
        // subnode[1] <= set
        // same as in previous `if`, but only for subnode[1]

        Node* new_node = new Node(this, new_set);
        new_node->subnodes[1] = subnodes[1];
        subnodes[1]->parent = new_node;
        subnodes[1] = new_node;
        return new_node;
    }

    // Step 4: (otherwise) General insert

    if (complement_sizes[0] <= complement_sizes[1]) {
        argmin_i = 0;
    } else {
        argmin_i = 1;
    }
    // We use `Insert` instead of `ExpandTo`-`InsertSubset` combination because __Occam's razor✨__
    return subnodes[argmin_i]->Insert(new_set);
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
