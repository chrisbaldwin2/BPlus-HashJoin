#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

const bool BOOL_GOOD = true;
const bool BOOL_BAD = false;

const int32_t INT_GOOD = 0;
const int32_t INT_BAD = -1;

const std::string STR_RETRY = "\0";
const std::string STR_NOT_FOUND = "\1";

const auto NODE_VALUES_SIZE = 32;
const auto NODE_CHILDREN_SIZE = NODE_VALUES_SIZE+1;
const auto LAST_NODE_INDEX = NODE_CHILDREN_SIZE-1;
const auto NODE_MID_INDEX = NODE_VALUES_SIZE/2;

// Used to indicate if the child is defined
#define EMPTY_PTR nullptr
// Used to indicate if the value is defined
const int32_t EMPTY_INT = -1;

const auto TUPLES_PER_BLOCK = 8;
const auto MEM_BLOCK_SIZE = 15;

const auto R1_TUPLES = 1000;
const auto R2_TUPLES = 5000;

const auto R1_KEY_INDEX = 1;
const auto R2_KEY_INDEX = 0;
const auto OUT_KEY_INDEX = 1;

typedef std::vector<int32_t> tuple;

struct block {
    std::vector<tuple> tuples;
    int key_index;

    int size() {
        return tuples.size();
    }

    bool full() {
        return size() >= TUPLES_PER_BLOCK;
    }

    void clear() {
        tuples.clear();
    }

    void add(tuple t) {
        tuples.push_back(t);
    }

    block(int key_index) {
        this->key_index = key_index;
        tuples.clear();
    }

    block(int key_index, std::vector<tuple> tuples) {
        this->key_index = key_index;
        this->tuples = tuples;
    }

    void print() {
        std::cout << "Block: " << std::endl;
        for(auto tuple : tuples){
            std::cout << "\t";
            for(auto value : tuple){
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }
};

#endif