#ifndef __POOL_H__
#define __POOL_H__

#include "client.hpp"
#include "test.hpp"
#include <vector>

class Pool
{
private:
    int pool_size;
    std::vector<Client*> clients;
public:
    Pool(int pool_size, std::string address, int port);
    bool warmup(Test *tester);
    bool run(Test *tester);
    bool warmup_thread(Test *tester, Client *client, int index);
    bool run_thread(Test *tester, Client *client, int index);
    int test(Test *tester, bool warmup=true);
    ~Pool();
};

#endif