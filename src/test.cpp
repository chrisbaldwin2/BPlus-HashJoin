#include "../include/test.hpp"
#include <string>
#include <random>
#include <iostream>



BTreeTest::BTreeTest(){
    this->btree = new BTree(13);
}

BTreeTest::~BTreeTest(){
    delete this->btree;
}

void BTreeTest::generate_nodes(int32_t num_nodes){

}

void BTreeTest::clean_tree(){
    delete this->btree;
}

void BTreeTest::build_dense_tree(int32_t order){

}

void BTreeTest::build_sparse_tree(int32_t order){

}

void BTreeTest::insert_random(int32_t num_values){

}

void BTreeTest::delete_random(int32_t num_values){

}

void BTreeTest::search_random(int32_t num_values){

}

bool BTreeTest::test_sparse_tree(){
    return true;
}

bool BTreeTest::test_dense_tree(){
    return true;
}
