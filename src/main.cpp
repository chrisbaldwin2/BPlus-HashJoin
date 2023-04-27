#include <iostream>
#include "../include/common.hpp"
#include "../include/node.hpp"
#include "../include/btree.hpp"
#include "../include/test.hpp"

int main(int argc, char** argv) {

    if(argc != 2){
        // for(int i = 1; i < argc; i++){
        //     std::cout << argv[i] << std::endl;
        // }
        std::cout << "Usage: ./main <test>" << std::endl;
        std::cout << "Tests: " << std::endl;
        std::cout << "\tbtree1: Tests the order 13 dense tree" << std::endl;
        std::cout << "\tbtree2: Tests the order 13 sparse tree" << std::endl;
        std::cout << "\tbtree3: Tests the order 24 dense tree" << std::endl;
        std::cout << "\tbtree4: Tests the order 24 sparse tree" << std::endl;
        std::cout << "\thash1: Tests 5.1 - R w/ 1000 tuples and using S's keys" << std::endl;
        std::cout << "\thash2: Tests 5.2 - R w/ 1200 tuples and using random keys = [20000, 30000)" << std::endl;
        std::cout << "\tall: Runs all tests" << std::endl;
        return 1;
    }
    std::string test = argv[1];
    if (test == "btree1"){
        BTreeTest* btree_test = new BTreeTest();
        btree_test->run();
        delete btree_test;
    } else if (test == "btree2"){
        BTreeTest* btree_test = new BTreeTest();
        btree_test->run();
        delete btree_test;
    } else if (test == "btree3"){
        BTreeTest* btree_test = new BTreeTest();
        btree_test->run();
        delete btree_test;
    } else if (test == "btree4"){
        BTreeTest* btree_test = new BTreeTest();
        btree_test->run();
        delete btree_test;
    } else if (test == "hash1"){
        HashTest* hash_test = new HashTest();
        hash_test->run_5_1();
        delete hash_test;
    } else if (test == "hash2"){
        HashTest* hash_test = new HashTest();
        hash_test->run_5_2();
        delete hash_test;
    } else if (test == "all"){
        BTreeTest* btree_test = new BTreeTest();
        btree_test->run();
        delete btree_test;
        HashTest* hash_test = new HashTest();
        hash_test->run();
        delete hash_test;
    } else {
        std::cout << "Invalid test: " << test << std::endl;
        return 1;
    }
    return 0;
}