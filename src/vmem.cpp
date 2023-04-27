
#include "../include/common.hpp"
#include "../include/vmem.hpp"
#include "../include/vdisk.hpp"

VMem::VMem(int max_blocks) {
    this->max_blocks = max_blocks;
}

VMem::~VMem() {
    data.clear();
}

int VMem::get_num_blocks() {
    return data.size();
}

int VMem::get_max_blocks() {
    return max_blocks;
}

void VMem::insert_block(std::string key, block b) {
    if (data.size() >= max_blocks && !block_exists(key)) {
        std::cout << "VMem is full " << data.size() << " " << max_blocks << std::endl;
        print();
        std::cout << "Inserting " << key << std::endl;
        b.print();
        throw std::runtime_error("VMem is full");
    }
    data.insert_or_assign(key, b);
    // print();
}

bool VMem::delete_block(std::string key) {
    // std::cout << "VMem Deleting " << key << std::endl;
    auto result = data.erase(key) > 0;
    // std::cout << result  << data.size() << std::endl;
    return result;
}

block VMem::get_block(std::string key) {
    return data.at(key);
}

void VMem::append_to_disk(VDisk* vdisk, std::string key) {
    // Check if key exists in vdisk
    // vdisk->print();
    if (!vdisk->relation_exists(key)) {
        // std::cout << "Writing new relation " << key << std::endl;
        vdisk->write_block(key, 0, data.at(key));
        return;
    }
    // std::cout << "Appending to relation " << key << std::endl;
    vdisk->append_block(key, data.at(key));
}

bool VMem::block_exists(std::string key) {
    return data.find(key) != data.end();
}

void VMem::clean() {
    data.clear();
}

void VMem::print() {
    for (auto [key, val] : data) {
        std::cout << key << ": " << std::endl;
        val.print();
    }
}