#ifndef __NODE_H__
#define __NODE_H__

#include <common.hpp>
class Node;

// Used for insert / deletion of B+ tree
struct pop_frame {
    Node* node;
    int32_t value;

    pop_frame(Node* node, int32_t value) {
        this->node = node;
        this->value = value;
    }
    pop_frame() {
        this->node = EMPTY_PTR;
        this->value = EMPTY_INT;
    }
    static bool compare(pop_frame mine, pop_frame your){
        return (mine.node == your.node) && (mine.value == your.value);
    }
    static bool less_than(pop_frame mine, pop_frame your){
        return (mine.value < your.value);
    }
    ~pop_frame(){
    }
};

struct split_frame {
    pop_frame left;
    pop_frame right;
    int32_t value;

    split_frame(pop_frame left, pop_frame right, int32_t value) {
        this->left = left;
        this->right = right;
        this->value = value;
    }
    split_frame() {
        this->left = pop_frame();
        this->right = pop_frame();
        this->value = EMPTY_INT;
    }
    ~split_frame(){}
};

struct range_search_frame {
    std::vector<pop_frame> results;

    range_search_frame(std::vector<pop_frame> results) {
        this->results = results;
    }
    range_search_frame() {
        this->results = std::vector<pop_frame>();
    }
    ~range_search_frame(){
        results.clear();
    }
};

/**
 * Node organization
 * 
 * Leaf nodes:
 * value_0, child_0, value_1, child_1, ..., value_N, child_N, next_node (child_N+1)
 * 
 * Branch nodes:
 * child_0, value_0, child_1, value_1, ..., child_N, value_N, child_N+1
 * 
 * Splitting a leaf node:
 *   node_0: value_0, child_0, value_1, child_1, ..., value_N/2, child_N/2
 *   node_1: value_N/2+1, child_N/2+1, value_N/2+2, child_N/2+2, ..., value_N, child_N
 *   pop_frame: node_1, value_N/2+1
 * 
 * Splitting a branch node:
 *   node_0: child_0, value_0, child_1, value_1, ..., value_N/2, child_N/2
 *   node_1: child_N/2+1, value_N/2+2, child_N/2+2, ..., child_N, value_N, child_N+1
 *   pop_frame: node_1, value_N/2+1
*/

class Node {
private: 
    int32_t order;
    int node_id;
    std::vector<Node*> children;
    std::vector<int32_t> values;
    Node* next;
    bool leaf;
    bool root;
public:
    Node(int32_t order);
    Node(int32_t order, bool leaf, bool root);
    Node(int32_t order, bool leaf, bool root, std::vector<pop_frame> tuples);
    ~Node();
    std::string to_string();
    pop_frame search(int32_t value);
    range_search_frame range_search(int32_t start, int32_t end);
    pop_frame insert(pop_frame new_tuple);
    pop_frame remove(int32_t value);
    pop_frame merge(pop_frame new_tuple);
    bool full();
    bool empty();
    bool underflow();
    bool overflow();
    bool is_leaf();
    bool is_root();
    void set_root(bool root);
    void set_leaf(bool leaf);
    int32_t get_order();
    int32_t get_num_children();
    int32_t get_num_values();
    int32_t get_min_value();
    void print();
    // Branch node functions
    int32_t search_child_index(int32_t value);
    pop_frame branch_split(pop_frame new_tuple);
    pop_frame branch_merge(pop_frame new_tuple);
    pop_frame branch_remove(int32_t value);
    pop_frame branch_insert(pop_frame new_tuple);
    // Leaf node functions
    int32_t contains(int32_t value);
    void add_value(int32_t value);
    bool remove_value(int32_t value);
    void move_value(int32_t index, Node* dest);
    range_search_frame leaf_range_search(int32_t start, int32_t end);
    pop_frame leaf_split(int32_t value);
    pop_frame leaf_merge(pop_frame new_tuple);
    pop_frame leaf_remove(int32_t value);
    pop_frame leaf_insert(pop_frame new_tuple);
    std::vector<pop_frame> get_range(int32_t end);
};

/** TODO:
 * - correct range search for order 
 * - branch merge
 * - leaf merge
 * - handle leaf merge from branch remove
*/

#endif