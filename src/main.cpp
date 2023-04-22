#include <iostream>
#include "../include/common.hpp"
#include "../include/node.hpp"
#include "../include/btree.hpp"
#include "../include/test.hpp"

int main(int argc, char** argv) {
    int32_t order = 13;
    BTree tree(order);
    for(int i = 0; i < 100; i++){
    tree.insert(pop_frame((void *) -1, i));
    tree.print();
    }
    return 0;
}