#include "../include/btree.hpp"
#include "../include/common.hpp"
#include <iostream>
#include <string>
#include <thread>


BTree::BTree(int32_t order){
    std::cout << "Tree constructor called" << std::endl;
    this->order = order;
    this->root = new Node(order, true, true);
    this->num_nodes = 0;
}

BTree::~BTree(){
    delete this->root;
}

void BTree::insert(int32_t value){
    std::cout << "Inserting " << value << std::endl;
    pop_frame result = this->root->insert(pop_frame(EMPTY_PTR, value));
    this->num_nodes++;
    if(result.node != EMPTY_PTR){
        std::cout << "Splitting root" << std::endl;
        std::vector<pop_frame> children;
        children.push_back(pop_frame(this->root, EMPTY_INT));
        children.push_back(pop_frame(result.node, result.value));
        Node* new_root = new Node(order, false, true, children);
        this->root->set_root(false);
        this->root = new_root;
    }
}

bool BTree::remove(int32_t value){
    std::cout << "Removing " << value << std::endl;
    pop_frame result = this->root->remove(value);
    assert(result.node == EMPTY_PTR);
    this->num_nodes--;
    return true;
}

void BTree::print(){
    std::cout << "Printing tree" << std::endl;
    this->root->print();
}

pop_frame BTree::search(int32_t value){
    std::cout << "Searching for " << value << std::endl;
    return this->root->search(value);
}

bool BTree::exists(int32_t value){
    std::cout << "Checking if " << value << " exists" << std::endl;
    return this->root->search(value).value == value;
}

range_search_frame BTree::range_search(int32_t start, int32_t end){
    std::cout << "Searching for range " << start << " to " << end << std::endl;
    return this->root->range_search(start, end);
}