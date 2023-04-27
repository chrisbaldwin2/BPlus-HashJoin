#ifndef __GENERATOR_H__
#define __GENERATOR_H__

#include "common.hpp"
#include <set>

class Generator {
private:
    std::set<int32_t> nodes;
public:
    Generator();
    ~Generator();
    void generate(int32_t num_nodes);
    void print();
    // Return must be deleted by the caller
    std::vector<int32_t>* get_random_nodes(int32_t num_nodes);
    std::vector<int32_t>* get_sequential_nodes(int32_t num_nodes);
    std::vector<int32_t>* get_outer_nodes(int32_t num_nodes);
    std::vector<int32_t>* get_nodes();
};

class TupleGenerator {
private:
    std::vector<tuple> r1_tuples;
    std::vector<tuple> r2_tuples;
    void generate(int32_t r1_tuples, int32_t r2_tuples, bool use_r1_keys);
public:
    TupleGenerator();
    ~TupleGenerator();
    void generate_5_1();
    void generate_5_2();
    std::vector<tuple>* get_r1_tuples();
    std::vector<tuple>* get_r2_tuples();
    std::vector<int32_t>* get_keys(int num_keys);
    void print();
};

#endif