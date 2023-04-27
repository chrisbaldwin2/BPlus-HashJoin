
#include "../include/common.hpp"
#include "../include/vdisk.hpp"

VDisk::VDisk() {
    this->num_ops = 0;
    this->relations = std::map<std::string, block>();
    this->block_directory = std::map<std::string, int>();
}

VDisk::~VDisk() {
    relations.clear();
    block_directory.clear();
}

int VDisk::get_num_ops() {
    return num_ops;
}

void VDisk::clear_num_ops() {
    num_ops = 0;
}

std::map<std::string, int> VDisk::get_block_directory() {
    return block_directory;
}

bool VDisk::relation_exists(std::string name) {
    // std::cout << "Checking if relation exists " << name << std::endl;
    return block_directory.find(name) != block_directory.end();
}

bool VDisk::block_exists(std::string name) {
    // std::cout << "Checking if block exists " << name << std::endl;
    return relations.find(name) != relations.end();
}

int VDisk::get_relation_size(std::string name) {
    if (!relation_exists(name)) {
        throw std::runtime_error("Relation does not exist: " + name);
    }
    return block_directory.at(name);
}

void VDisk::delete_relation(std::string name) {
    if (!relation_exists(name)) {
        throw std::runtime_error("Relation does not exist");
    }
    int max_index = block_directory.at(name);
    for (int i = 0; i <= max_index; i++) {
        std::string offset_name = name + "_" + std::to_string(i);
        relations.erase(offset_name);
    }
    block_directory.erase(name);
}

void VDisk::write_block(std::string name, int32_t offset, block data) {
    // std::cout << "Writing block " << name << "_" << offset << std::endl;
    if (!block_exists(name)) {
        // Insert the relation into the block directory
        block_directory.insert_or_assign(name, 0);
    }
    std::string offset_name = name + "_" + std::to_string(offset);
    relations.insert_or_assign(offset_name, data);
    block_directory.at(name) = std::max(offset+1, block_directory.at(name));
    num_ops++;
}

void VDisk::write_block_schedule(std::string name, int32_t offset, std::vector<block> data) {
    for (int i = 0; i < data.size(); i++) {
        write_block(name, offset + i, data.at(i));
    }
}

void VDisk::append_block(std::string name, block data) {
    int32_t offset = block_directory.at(name);
    write_block(name, offset, data);
}

block VDisk::read_block(std::string name, int32_t offset) {
    std::string offset_name = name + "_" + std::to_string(offset);
    if (!block_exists(offset_name)) {
        throw std::runtime_error("Block does not exist: " + offset_name);
    }
    return relations.at(offset_name);
}

std::vector<block> VDisk::read_block_schedule(std::string name, int32_t offset, int32_t size) {
    if (!relation_exists(name)) {
        throw std::runtime_error("Relation does not exist: " + name);
    }
    std::vector<block> result;
    int32_t max_index = block_directory.at(name);
    for (int i = offset; i < max_index; i++) {
        std::string offset_name = name + "_" + std::to_string(i);
        // std::cout << "Reading block " << offset_name << std::endl;
        result.push_back(relations.at(offset_name));
        if (size != -1 && result.size() == size) {
            break;
        }
    }
    return result;
}

void VDisk::clean() {
    relations.clear();
    block_directory.clear();
}

void VDisk::print(std::string name) {
    std::cout << "VDisk Printing " << name << std::endl;
    if (!relation_exists(name)) {
        throw std::runtime_error("Relation does not exist: " + name);
    }
    int32_t max_index = block_directory.at(name);
    for (int i = 0; i < max_index; i++) {
        std::string offset_name = name + "_" + std::to_string(i);
        if(!block_exists(offset_name))
            continue;
        std::cout << offset_name << " : ";
        relations.at(offset_name).print();
    }
}

void VDisk::print_directory(){
    std::cout << "Block Directory" << std::endl;
    for(auto const& [relation, size] : block_directory){
        std::cout << relation << " : " << size << std::endl;
    }
}

void VDisk::print_relations_names(){
    std::cout << "Relation Names" << std::endl;
    for(auto const& [relation, size] : block_directory){
        std::cout << relation << std::endl;
    }
}

void VDisk::print() {
    std::cout << "VDisk Printing" << std::endl;
    print_directory();
    print_relations_names();
    for (auto const& [relation, size] : block_directory) {
        std::cout << relation << " : " << size << std::endl;
        print(relation);
    }
}



