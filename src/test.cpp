#include "../include/test.hpp"
#include <string>
#include <random>
#include <iostream>

Test::Test()
{
    std::cout << "Test: default constructor" << std::endl;
}

Test::Test(int num_objs)
{
    this->num_objs = num_objs;
}

Test::Test(float set_ratio, int num_objs)
{
    this->set_ratio = set_ratio;
    this->num_objs = num_objs;
    std::cout << "Test: set_ratio " << this->set_ratio << " num_objs " << this->num_objs << std::endl;
}

bool Test::is_set()
{
    return rand()%100 < set_ratio*100;
}

int Test::get_num_objs()
{
    return this->num_objs;
}


ETCTest::ETCTest(int num_objs) : Test(0.05, num_objs)
{
    this->num_objs = num_objs;
    std::cout << "ETCTest: num_objs " << this->num_objs << std::endl;
}

std::tuple<int, int> ETCTest::key_range(int index)
{
    return std::make_tuple(16, 16);
}

std::tuple<int, int> ETCTest::value_range(int index)
{
    if (index < 0.9 * num_objs) {
        return std::make_tuple(16, 512);
    } else {
        return std::make_tuple(512, 10000);
    }
}

std::string ETCTest::get_name()
{
    return "ETCTest";
}

int ETCTest::key_size(int index)
{
    return 16;
}

int ETCTest::value_size(int index)
{
    if (index < 0.9 * num_objs) {
        return 16 + rand() % 496;
    } else {
        return 512 + rand() % 9450;
    }
}

std::string ETCTest::key(int index)
{
    std::string key = std::to_string(index);
    key = std::string(16 - key.length(), '0') + key;
    return key;
}

std::string ETCTest::value(int index)
{
    std::string value = "";
    for (int i = 0; i < value_size(index); i+=4) {
        int rand_int = rand();
        char c;
        // i + 0 must be less than value_size(index)
        c = (rand_int & 0xFF) % 26 + 'a';
        value += c;
        if(i + 1 >= value_size(index)) break;
        c = ((rand_int >> 8) & 0xFF) % 26 + 'a';
        value += c;
        if(i + 2 >= value_size(index)) break;
        c = ((rand_int >> 16) & 0xFF) % 26 + 'a';
        value += c;
        if(i + 3 >= value_size(index)) break;
        c = ((rand_int >> 24) & 0xFF) % 26 + 'a';
        value += c;
    }
    return value;
}

ETCTest::~ETCTest(){}

SYSTest::SYSTest(int num_objs) : Test(0.25, num_objs)
{
    this->num_objs = num_objs;
    std::cout << "SYSTest: num_objs " << this->num_objs << std::endl;
}

std::string SYSTest::get_name()
{
    return "SYSTest";
}

std::tuple<int, int> SYSTest::key_range(int index)
{
    return std::make_tuple(16, 20);
}

std::tuple<int, int> SYSTest::value_range(int index)
{
    if (index < 0.8 * num_objs) {
        return std::make_tuple(320, 500);
    } else if (index < 0.88 * num_objs) {
        return std::make_tuple(16, 320);
    } else {
        return std::make_tuple(512, 10000);
    }
}

int SYSTest::key_size(int index)
{
    if (index < 0.4 * num_objs) {
        return 16 + rand() % 4;
    } else {
        return 20 + rand() % 25;
    }
}

int SYSTest::value_size(int index)
{
    if (index < 0.8 * num_objs) {
        return 320 + rand() % 180;
    } else if (index < 0.88 * num_objs) {
        return 16 + rand() % 304;
    } else {
        return 512 + rand() % 9450;
    }
}

std::string SYSTest::key(int index)
{
    std::string key = std::to_string(index);
    key = std::string("0", 16 - key.length()) + key;
    return key;
}

std::string SYSTest::value(int index)
{
    std::string value;
    for (int i = 0; i < value_size(index); i+=4) {
        int rand_int = rand();
        char c;
        // i + 0 must be less than value_size(index)
        c = rand_int & 0xFF % 26 + 'a';
        value += c;
        if(i + 1 >= value_size(index)) break;
        c = (rand_int >> 8) & 0xFF % 26 + 'a';
        value += c;
        if(i + 2 >= value_size(index)) break;
        c = (rand_int >> 16) & 0xFF % 26 + 'a';
        value += c;
        if(i + 3 >= value_size(index)) break;
        c = (rand_int >> 24) & 0xFF % 26 + 'a';
        value += c;
    }
    return value;
}

SYSTest::~SYSTest(){}