#ifndef __VDISK_H__
#define __VDISK_H__

#include "common.hpp"
#include <string>
#include <map>


class VDisk {
private:
    // Might need to use block pointers instead of blocks for heap allocation
    std::map<std::string, block> relations;
    // Holds the index of the last block in the relation
    std::map<std::string, int> block_directory;
    int num_ops;
public:
    VDisk();
    ~VDisk();
    int get_num_ops();
    void clear_num_ops();
    std::map<std::string, int> get_block_directory();
    bool relation_exists(std::string name);
    bool block_exists(std::string name);
    int get_relation_size(std::string name);
    void delete_relation(std::string name);
    void write_block(std::string name, int32_t offset, block data);
    void write_block_schedule(std::string name, int32_t offset, std::vector<block> data);
    void append_block(std::string name, block data);
    block read_block(std::string name, int32_t offset);
    // If size is -1, read until the end of the relation
    std::vector<block> read_block_schedule(std::string name, int32_t offset, int32_t size = -1);
    void clean();
    void print();
    void print(std::string name);
    void print_directory();
    void print_relations_names();
};

#endif