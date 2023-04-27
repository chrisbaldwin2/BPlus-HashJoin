#ifndef __BTREE_H__
#define __BTREE_H__

#include "node.hpp"
#include "generator.hpp"

// The B+ tree class
class BTree
{
private:
    Node* root;
    int32_t order;
    int32_t num_nodes;
public:
    BTree(int32_t order);
    ~BTree();
    void insert(int32_t value);
    bool remove(int32_t value);
    void print();
    pop_frame search(int32_t value);
    bool exists(int32_t value);
    range_search_frame range_search(int32_t start, int32_t end);
};

#endif