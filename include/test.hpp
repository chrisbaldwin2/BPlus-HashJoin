#ifndef __TEST_H__
#define __TEST_H__

#include <string>
#include <random>

class Test
{
private:
    float set_ratio;
    int num_objs;
public:
    Test(); // default constructor (for inheritance)
    Test(int num_objs);
    Test(float set_ratio, int num_objs);
    int get_num_objs();
    bool is_set();
    virtual std::tuple<int, int> key_range(int index) = 0;
    virtual std::tuple<int, int> value_range(int index) = 0;
    virtual std::string get_name() = 0;
    virtual std::string key(int index) = 0;
    virtual std::string value(int index) = 0;
    ~Test(){};
};

/**
 *  ETC:
 *  5% SET
 *  95% GET 
 *  Key size is 16 bytes
 *  90% values evenly distributed between 16 and 512 bytes
 *  10% values between 512-10_000 bytes
*/
class ETCTest : public Test
{
private:
    float set_ratio; // 0.05
    int num_objs;
public:
    ETCTest() : Test(){};
    ETCTest(int num_objs);
    std::tuple<int, int> key_range(int index);
    std::tuple<int, int> value_range(int index);
    std::string get_name();
    int key_size(int index);
    int value_size(int index);
    std::string key(int index);
    std::string value(int index);
    ~ETCTest();
};

/**
 *  SYS: 
 *  25% SET 
 *  75% GET
 *  
 *  40% key are 16-20 bytes
 *  60% keys are 20-45 bytes
 *  
 *  80% of values are between 320-500 bytes
 *  8% are smaller
 *  12% are between 500-10_000 bytes
*/
class SYSTest : public Test
{
private:
    float set_ratio; // 0.25
    int num_objs;
public:
    SYSTest() : Test(){};
    SYSTest(int num_objs);
    std::tuple<int, int> key_range(int index);
    std::tuple<int, int> value_range(int index);
    std::string get_name();
    int key_size(int index);
    int value_size(int index);
    std::string key(int index);
    std::string value(int index);
    ~SYSTest();
};

#endif