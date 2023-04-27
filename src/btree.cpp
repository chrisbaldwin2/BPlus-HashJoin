#include "../include/btree.hpp"
#include "../include/common.hpp"
#include <iostream>
#include <string>
#include <thread>


BTree::BTree(int32_t order){
    // std::cout << "Tree constructor called" << std::endl;
    this->order = order;
    this->root = new Node(order, true, true);
    this->print_flag = false;
}

BTree::~BTree(){
    delete this->root;
}

void BTree::set_print_flag(bool flag){
    this->print_flag = flag;
    this->root->set_print_flag(flag);
}

void BTree::insert(int32_t value){
    if(this->print_flag)
        std::cout << "Inserting " << value << std::endl;
    pop_frame result = this->root->insert(pop_frame(EMPTY_PTR, value));
    if(result.node != EMPTY_PTR){
        if(this->print_flag){
            std::cout << "Splitting root " << root->to_string() << std::endl;
            std::cout << "Splitting result " << result.node->to_string() << std::endl;
        }
        std::vector<pop_frame> children;
        children.push_back(pop_frame(this->root, EMPTY_INT));
        children.push_back(pop_frame(result.node, result.value));
        Node* new_root = new Node(order, false, true, children);
        this->root->set_root(false);
        this->root = new_root;
        if(this->print_flag) {
            std::cout << "New root " << this->root->to_string() << std::endl;
            std::cout << "Left child " << root->to_string() << std::endl;
            std::cout << "Right child " << result.node->to_string() << std::endl;
        }
    }
}

bool BTree::remove(int32_t value){
    if(this->print_flag)
        std::cout << "Removing " << value << std::endl;
    pop_frame result = this->root->remove(value);
    assert(result.node == EMPTY_PTR);
    return true;
}

void BTree::print(){
    std::cout << "Printing tree" << std::endl;
    this->root->print();
}

pop_frame BTree::search(int32_t value){
    if(this->print_flag)
        std::cout << "Searching for " << value << std::endl;
    return this->root->search(value);
}

bool BTree::exists(int32_t value){
    if(this->print_flag)
        std::cout << "Checking if " << value << " exists" << std::endl;
    return this->root->search(value).value == value;
}

range_search_frame BTree::range_search(int32_t start, int32_t end){
    if(this->print_flag)
        std::cout << "Searching for range " << start << " to " << end << std::endl;
    return this->root->range_search(start, end);
}