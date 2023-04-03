#ifndef __COMMON_H__
#define __COMMON_H__

#include <string>

const auto BOOL_GOOD = true;
const auto BOOL_BAD = false;

const auto INT_GOOD = 0;
const auto INT_BAD = -1;

const std::string STR_RETRY = "\0";
const std::string STR_NOT_FOUND = "\1";

const auto MAX_RETRY = 10;

// Errors out if x is false
#define CHECK_SET(x) if(!x) { std::cerr << "Error setting key" << std::endl; return INT_BAD; }
// Errors out if x is empty string
#define CHECK_GET(x) if(!x.length()) { std::cerr << "Error getting key" << std::endl; return INT_BAD; }
// Errors out if x is true
#define CHECK_TEST(x) if(x == INT_BAD) { std::cerr << "Error testing" << std::endl; return INT_BAD; }
// Errors out if x is false
#define CHECK_RESULT(x) if(x == BOOL_BAD) { std::cerr << "Error bad result" << std::endl; return INT_BAD; }

#endif