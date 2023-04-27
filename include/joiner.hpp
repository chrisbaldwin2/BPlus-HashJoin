#ifndef __JOINER_H__
#define __JOINER_H__

#include "common.hpp"
#include "vmem.hpp"
#include "vdisk.hpp"

class Joiner {
private:
    VMem* vmem;
    VDisk* vdisk;
    int buckets;
    bool print_flag;
    int num_buckets(int num_tuples);
    std::string get_bucket_name(std::string relation, int bucket_num);
    std::string get_block_name(std::string relation, int bucket_num, int block_num);
    void hash_to_buckets(std::string relation, int num_buckets);
    void merge_all_buckets(std::string relation1, std::string relation2, std::string output);
    void merge_buckets(std::string relation1, std::string relation2, std::string output, int bucket_num);
    std::vector<tuple> join_tuples(std::vector<tuple> tuples1, std::vector<tuple> tuples2, int r1_key_index = 1, int r2_key_index = 0);
    int get_key_index(std::string relation);
public:
    // Assume that the vmem and vdisk are already initialized
    Joiner(VMem* vmem, VDisk* vdisk);
    ~Joiner();
    void write_relations_to_disk(std::string relation1, std::vector<tuple>* r1_tuples, std::string relation2, std::vector<tuple>* r2_tuples);
    void join(std::string relation1, std::string relation2, std::string output);
    void set_print_flag(bool flag);
    void print();
    void print(std::string relation);
    void print(std::string relation, std::vector<int> keys);
};

/** TODO:
 * 
*/

#endif