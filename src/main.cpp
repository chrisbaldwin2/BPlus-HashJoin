#include "../include/client.hpp"
#include "../include/pool.hpp"
#include "../include/test.hpp"
#include "../include/common.hpp"
#include<string>
#include<iostream>

int client_test(std::string ip, int port) {

    Client c(
        ip,
        port
    );
    CHECK_SET(c.set("foo", "bar"));
    CHECK_GET(c.get("foo"));
    CHECK_SET(c.set("foo", "buz"));
    CHECK_GET(c.get("foo"));
    std::cout << "Client Test passed" << std::endl;
    return INT_GOOD;
}

int tester_test(std::string ip, int port) {
    ETCTest etc(
        100
    );
    bool result;
    result = etc.key_size(0) == 16;
    CHECK_RESULT(result);
    result = etc.value_size(0) >= 16 && etc.value_size(0) < 512;
    CHECK_RESULT(result);
    std::cout << "Key: " << etc.key(0) << std::endl;
    std::cout << "Value: " << etc.value(0) << std::endl;
    result = etc.key(0) == "0000000000000000";
    CHECK_RESULT(result);
    std::cout << "Tester Test passed" << std::endl;
    return INT_GOOD;
}

int pool_test(std::string ip, int port, int pool_size = 1, int num_objs = 100) {
    std::cout << "Creating pool" << std::endl;
    Pool *p = new Pool(
        pool_size,
        ip,
        port
    );
    std::cout << "Creating test" << std::endl;
    ETCTest *t = new ETCTest(num_objs);
    std::cout << "Test objs: " << t->get_num_objs() << std::endl;
    std::cout << "Running test" << std::endl;
    CHECK_TEST(p->test(t));
    std::cout << "Pool Test passed" << std::endl;
    return INT_GOOD;
}

std::tuple<std::string, int> get_ip_port(int argc, char** argv) {
    // Get current ip address
    std::string ip = "169.254.59.35";
    int port = 11211;
    if (argc > 3) {
        std::cerr << "Usage: " << argv[0] << " [ip] [port]" << std::endl;
        return std::make_tuple("", -1);
    }
    if (argc > 1) {
        ip = argv[1];
    }
    if (argc > 2) {
        port = atoi(argv[2]);
    }
    return std::make_tuple(ip, port);
}


int main(int argc, char** argv) {
    // Unpack ip and port
    std::string ip;
    int port;
    std::tie(ip, port) = get_ip_port(argc, argv);
    // CHECK_TEST(client_test(ip, port));
    // CHECK_TEST(tester_test(ip, port));
    CHECK_TEST(pool_test(ip, port, 16, 1000000));
    return 0;
}