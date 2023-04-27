#ifndef __TEST_H__
#define __TEST_H__

#include <string>
#include <random>
#include <common.hpp>
#include <btree.hpp>
#include <generator.hpp>
#include <joiner.hpp>

int basic_tree_test();
int basic_gen_test();

class BTreeTest
{
private:
    BTree* btree;
    Generator* generator;
    int32_t order;
    void set_order(int32_t order);
    void generate_nodes(int32_t num_nodes);
    void clean_tree();
    void build_dense_tree();
    void build_sparse_tree();
    void insert_random(int32_t num_values);
    void delete_random(int32_t num_values);
    void search_random(int32_t num_values);
    void range_search_random(int32_t num_values);
    void random_random(int32_t num_values);
    bool test_sparse_tree();
    bool test_dense_tree();
public:
    BTreeTest();
    ~BTreeTest();
    bool run();
    bool run_1();
    bool run_2();
    bool run_3();
    bool run_4();
};

class HashTest
{
private:
    Joiner* joiner;
    TupleGenerator* generator;
    VMem* vmem;
    VDisk* vdisk;
    std::string r1_name;
    std::string r2_name;
    std::string output_name;

    void clean_joiner();
    void build_joiner();
    void write_tuples_to_disk();
    void print();
public:
    HashTest();
    ~HashTest();
    bool run_5_1();
    bool run_5_2();
    bool run();
};


#endif