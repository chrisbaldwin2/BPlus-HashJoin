#ifndef __NODE_H__
#define __NODE_H__

#include <stdint.h>
#include <vector>

const auto NODE_VALUES_SIZE = 256;
const auto NODE_CHILDREN_SIZE = NODE_VALUES_SIZE+1;

// Used to indicate if the child is defined
#define EMPTY_PTR nullptr
// Used to indicate if the value is defined
const int32_t EMPTY_INT = -1;

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
} split_frame;

typedef struct range_search_frame_struct {
    std::vector<pop_frame> results;

    range_search_frame_struct(std::vector<pop_frame> results) {
        this->results = results;
    }
    range_search_frame_struct() {
        this->results = std::vector<pop_frame>();
    }
} range_search_frame;

class Node {
private: 
    void* children[NODE_CHILDREN_SIZE];
    int32_t values[NODE_VALUES_SIZE];
public:
    Node();
    ~Node();
    virtual split_frame split(pop_frame new_tuple) = 0;
    virtual pop_frame search(int32_t value) = 0;
    virtual range_search_frame range_search(int32_t start, int32_t end) = 0;
    virtual pop_frame insert(pop_frame new_tuple) = 0;
    virtual pop_frame remove(pop_frame new_tuple) = 0;
};

class BranchNode : Node {
private:
    void* children[NODE_CHILDREN_SIZE];
    int32_t values[NODE_VALUES_SIZE];
public:
    BranchNode();
    ~BranchNode();
};

class LeafNode : Node {
private:
    void* children[NODE_VALUES_SIZE];
    int32_t values[NODE_VALUES_SIZE];
    LeafNode* nextNode;
public:
    LeafNode();
    ~LeafNode();
};

#endif