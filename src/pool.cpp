#include "../include/pool.hpp"
#include "../include/common.hpp"
#include <iostream>
#include <string>
#include <thread>

Pool::Pool(int pool_size, std::string address, int port)
{
    this->pool_size = pool_size;
    for (int i = 0; i < pool_size; i++) {
        std::cout << "Creating client " << i << std::endl;
        Client *c = new Client(address, port);
        clients.push_back(c);
    }
}

bool Pool::warmup_thread(Test *tester, Client *client, int index)
{
    int i;
    // std::cout << "index " << index << " objs " << tester->get_num_objs() << " pool_size " << pool_size << std::endl;
    for(i = index; i < tester->get_num_objs(); i+=pool_size) {
        std::string key = tester->key(i);
        std::string value = tester->value(i);
        if (!client->set(key, value, 0, false)) {
            std::cout << "Error: set failed" << std::endl;
            return BOOL_BAD;
        }
        // std::cout << "Thread " << index << " finished " << i << std::endl;
    }
    std::cout << "Warmup thread " << index << " finished" << std::endl;
    return BOOL_GOOD;
}

bool Pool::run_thread(Test *tester, Client *client, int index)
{
    int i;
    const int num_objs = tester->get_num_objs();
    for(i = index; i < num_objs; i+=pool_size) {
        std::string key = tester->key(i);
        if(tester->is_set()){
            std::string value = tester->value(i);
            if (!client->set(key, value)) {
                std::cout << "Error: set failed" << std::endl;
                return BOOL_BAD;
            }
        } else {
            std::string value = client->get(key);
            if (value == STR_NOT_FOUND) continue;
            if (value == "") {
                std::cout << "Error: get empty" << std::endl;
                return BOOL_BAD;
            }
            // Check that value is within range
            int lower, upper;
            std::tie(lower, upper) = tester->value_range(i);
            if (value.length() < lower || value.length() > upper) {
                std::cout << "Error: value length out of range" << std::endl;
                return BOOL_BAD;
            }

        }
        // std::cout << "Thread " << index << " finished " << i << std::endl;
    }
    std::cout << "Test thread " << index << " finished " << index << std::endl;
    return BOOL_GOOD;
}

bool Pool::warmup(Test *tester)
{
    if(pool_size == 1){
        return warmup_thread(tester, clients[0], 0);
    }
    // Spawn threads
    std::vector<std::thread> threads;
    for (int i = 0; i < pool_size; i++) {
        threads.push_back(std::thread(&Pool::warmup_thread, this, tester, clients[i], i));
    }
    // Wait for threads to finish
    for (int i = 0; i < pool_size; i++) {
        threads[i].join();
    }
    return BOOL_GOOD;
}

bool Pool::run(Test *tester)
{
    if(pool_size == 1){
        return run_thread(tester, clients[0], 0);
    }
    // Spawn threads
    std::vector<std::thread> threads;
    for (int i = 0; i < pool_size; i++) {
        threads.push_back(std::thread(&Pool::run_thread, this, tester, clients[i], i));
    }
    // Wait for threads to finish
    for (int i = 0; i < pool_size; i++) {
        threads[i].join();
    }
    return BOOL_GOOD;
}

int Pool::test(Test *tester, bool warmup)
{
    std::cout << "Testing " << tester->get_name() << std::endl;
    std::cout << "Pool size: " << pool_size << std::endl;
    std::cout << "Num objs: " << tester->get_num_objs() << std::endl;
    if (warmup && this->warmup(tester) == BOOL_BAD) {
        return INT_BAD;
    }
    if(run(tester) == BOOL_BAD){
        return INT_BAD;
    }
    return INT_GOOD;
}

Pool::~Pool()
{
    for (int i = 0; i < pool_size; i++) {
        delete clients[i];
    }
    clients.clear();
}