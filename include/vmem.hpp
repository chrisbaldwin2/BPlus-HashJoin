#ifndef __VMEM_H__
#define __VMEM_H__

#include "common.hpp"
#include "vdisk.hpp"

class VMem {
private:
    int max_blocks;
    std::map<std::string, block> data;
public:
    VMem(int max_blocks);
    ~VMem();
    int get_num_blocks();
    int get_max_blocks();
    void insert_block(std::string key, block b);
    bool delete_block(std::string key);
    block get_block(std::string key);
    void append_to_disk(VDisk* vdisk, std::string key);
    bool block_exists(std::string key);
    void clean();
    void print();
};

#endif