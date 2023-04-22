#ifndef __TEST_H__
#define __TEST_H__

#include <string>
#include <random>
#include <common.hpp>
#include <btree.hpp>

class BTreeTest
{
private:
    BTree* btree;
    std::vector<int32_t> nodes;
public:
    BTreeTest(); // default constructor (for inheritance)
    ~BTreeTest();
    void generate_nodes(int32_t num_nodes);
    void clean_tree();
    void build_dense_tree(int32_t order = 13);
    void build_sparse_tree(int32_t order = 13);
    void insert_random(int32_t num_values);
    void delete_random(int32_t num_values);
    void search_random(int32_t num_values);
    bool test_sparse_tree();
    bool test_dense_tree();
};



#endif