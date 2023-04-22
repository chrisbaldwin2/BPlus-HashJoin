#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>
#include <stdint.h>

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

#endif