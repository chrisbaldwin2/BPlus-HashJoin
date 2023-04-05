#ifndef __BTREE_H__
#define __BTREE_H__

#include <node.hpp>

// The B+ tree class
class BTree
{
private:
    Node* root;
public:
    BTree();
    ~BTree();
    int32_t insert(int32_t value);
    int32_t remove(int32_t value);
    int32_t search(int32_t value);
    int32_t range_search(int32_t start, int32_t end);
};

# endif