#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include <vector>
#include <common.hpp>

// Used for insert / deletion of B+ tree
typedef struct pop_frame_struct {
    void* node;
    int32_t value;

    pop_frame_struct(void* node, int32_t value) {
        this->node = node;
        this->value = value;
    }
    pop_frame_struct() {
        this->node = EMPTY_PTR;
        this->value = EMPTY_INT;
    }
    static bool compare(pop_frame_struct mine, pop_frame_struct your){
        return (mine.node == your.node) && (mine.value == your.value);
    }
    static bool less_than(pop_frame_struct mine, pop_frame_struct your){
        return (mine.value < your.value);
    }
    ~pop_frame_struct(){
    }
} pop_frame;

typedef struct split_frame_struct {
    pop_frame left;
    pop_frame right;
    int32_t value;

    split_frame_struct(pop_frame left, pop_frame right, int32_t value) {
        this->left = left;
        this->right = right;
        this->value = value;
    }
    split_frame_struct() {
        this->left = pop_frame();
        this->right = pop_frame();
        this->value = EMPTY_INT;
    }
    ~split_frame_struct(){}
} split_frame;

typedef struct range_search_frame_struct {
    std::vector<pop_frame> results;

    range_search_frame_struct(std::vector<pop_frame> results) {
        this->results = results;
    }
    range_search_frame_struct() {
        this->results = std::vector<pop_frame>();
    }
    ~range_search_frame_struct(){
        results.clear();
    }
} range_search_frame;

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
    void** children;
    int32_t* values;
    bool leaf;
    bool root;
public:
    Node(int32_t order);
    Node(int32_t order, bool leaf, bool root);
    Node(int32_t order, bool leaf, bool root, std::vector<pop_frame> tuples);
    ~Node();
    pop_frame branch_split(pop_frame new_tuple);
    pop_frame leaf_split(pop_frame new_tuple);
    pop_frame search(int32_t value);
    range_search_frame range_search(int32_t start, int32_t end);
    pop_frame insert(pop_frame new_tuple);
    pop_frame branch_merge(pop_frame new_tuple);
    pop_frame leaf_merge(pop_frame new_tuple);
    pop_frame remove(int32_t value);
    bool full();
    bool empty();
    bool underflow();
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
    void remove_child(void* child);
    // Leaf node functions
    int32_t contains(int32_t value);
    void add(pop_frame new_tuple);
    void remove_value(int32_t value);
    void move_value(int32_t index, Node* dest);
    std::vector<pop_frame> get_range(int32_t end);
};

#endif