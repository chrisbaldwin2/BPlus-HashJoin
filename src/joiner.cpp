
#include "../include/common.hpp"
#include "../include/joiner.hpp"
#include <cmath>


Joiner::Joiner(VMem* vmem, VDisk* vdisk){
    this->vmem = vmem;
    this->vdisk = vdisk;
    this->buckets = 0;
}

Joiner::~Joiner(){

}

/**
 * Want to have the number of buckets be the square root of the number of blocks
*/
int Joiner::num_buckets(int num_tuples){
    return std::ceil(std::sqrt(num_tuples));
}

std::string Joiner::get_bucket_name(std::string relation, int bucket_num){
    return relation + "_bucket" + std::to_string(bucket_num);
}

std::string Joiner::get_block_name(std::string relation, int bucket_num, int block_num){
    return relation + "_bucket" + std::to_string(bucket_num) + "_" + std::to_string(block_num);
}

void Joiner::hash_to_buckets(std::string relation, int num_buckets){
    // One input block 
    // num_buckets output blocks
    std::vector<block> blocks = vdisk->read_block_schedule(relation, 0);
    int key_index = blocks[0].key_index;
    std::vector<block> buckets(num_buckets, block(key_index));
    // std::cout << "Printing buckets " << num_buckets << " " << buckets.size() << std::endl;
    // for(auto b : buckets){
    //     b.print();
    // }
    for(auto block : blocks){
        for(int i = 0; i < TUPLES_PER_BLOCK; i++){
            int bucket_num = std::hash<int>()(block.tuples.at(i).at(key_index)) % num_buckets;
            std::string bucket_name = get_bucket_name(relation, bucket_num);
            buckets[bucket_num].tuples.push_back(block.tuples.at(i));
            if(buckets[bucket_num].tuples.size() == TUPLES_PER_BLOCK){
                vmem->insert_block(bucket_name, buckets[bucket_num]);
                vmem->append_to_disk(vdisk, bucket_name);
                buckets[bucket_num].tuples.clear();
                vmem->delete_block(bucket_name);
            }
        }
    }
    // Write remaining blocks to disk
    for(int i = 0; i < buckets.size(); i++){
        if(buckets[i].tuples.size() > 0){
            vmem->insert_block(get_bucket_name(relation, i), buckets[i]);
            vmem->append_to_disk(vdisk, get_bucket_name(relation, i));
            vmem->delete_block(get_bucket_name(relation, i));
        }
    }
}

void Joiner::merge_all_buckets(std::string relation1, std::string relation2, std::string output){
    for(int i = 0; i < buckets; i++){
        merge_buckets(relation1, relation2, output, i);
    }

}

void Joiner::set_print_flag(bool flag){
    print_flag = flag;
}

void Joiner::merge_buckets(std::string relation1, std::string relation2, std::string output, int bucket_num){
    if(vmem->get_num_blocks() > 1){
        vmem->print();
        std::cout.flush();
        throw std::runtime_error("VMem has more than one element before merge  " + std::to_string(vmem->get_num_blocks()));
    } // Make sure we have room in vmem
    if(vmem->get_num_blocks() == 0){
        vmem->insert_block(output, block(OUT_KEY_INDEX));
    }
    // Find the smaller bucket, read it into vmem
    // For the larger bucket, read in a single block at a time, merge, and write the output to vmem / vdisk
    std::string bucket1_name = get_bucket_name(relation1, bucket_num);
    std::string bucket2_name = get_bucket_name(relation2, bucket_num);
    // Read in the smaller bucket

    std::vector<block> bucket1_blocks = vdisk->read_block_schedule(bucket1_name, 0);
    std::vector<block> bucket2_blocks = vdisk->read_block_schedule(bucket2_name, 0);
    if(print_flag) {
        std::cout << "Bucket 1 blocks: " << bucket1_blocks.size() << std::endl;
        std::cout << "Bucket 2 blocks: " << bucket2_blocks.size() << std::endl;
    }
    // vmem->print();
    std::vector<block>* mem_blocks;
    std::vector<block>* disk_blocks;
    std::string smaller_bucket_name;
    std::string larger_bucket_name;
    if(bucket1_blocks.size() > bucket2_blocks.size()){
        // Read bucket2 into vmem
        mem_blocks = &bucket2_blocks;
        disk_blocks = &bucket1_blocks;
        smaller_bucket_name = relation2;
        larger_bucket_name = relation1;
    } else {
        // Read bucket1 into vmem
        mem_blocks = &bucket1_blocks;
        disk_blocks = &bucket2_blocks;
        smaller_bucket_name = relation1;
        larger_bucket_name = relation2;
    }
    // Write mem_blocks to vmem
    for(int i = 0; i < mem_blocks->size(); i++){
        vmem->insert_block(get_block_name(smaller_bucket_name, bucket_num, i), mem_blocks->at(i));
    }
    if(print_flag)
        vmem->print();
    block output_block = vmem->get_block(output);
    // Merge blocks
    std::vector<tuple> r1_tuples = {};
    std::vector<tuple> r2_tuples = {};
    for(auto block : *mem_blocks){
        for(auto tuple : block.tuples){
            r1_tuples.push_back(tuple);
        }
    }
    std::vector<tuple> new_tuples;
    for(int i = 0; i < disk_blocks->size(); i++){
        block b = disk_blocks->at(i);
        if(print_flag){
            std::cout << larger_bucket_name << "_bucket" << bucket_num << "_" << i << ":" << std::endl; b.print();
        }
        for(auto tuple : b.tuples){
            r2_tuples.push_back(tuple);
        }
        if(bucket1_blocks.size() > bucket2_blocks.size()){
            new_tuples = join_tuples(r2_tuples, r1_tuples);
        } else {
            new_tuples = join_tuples(r1_tuples, r2_tuples);
        }
        // Pop tuples off of new_tuples and append to output_tuples
        if(new_tuples.size() == 0)
            continue;
        while(new_tuples.size() > 0){
            if(output_block.full()){
                // Write output_block to disk
                vmem->insert_block(output, output_block);
                vmem->append_to_disk(vdisk, output);
                vmem->delete_block(output);
                output_block.clear();
            }
            output_block.add(new_tuples.back());
            new_tuples.pop_back();
        }
        r2_tuples.clear();
    }
    // Clean up mem_blocks
    for(int i = 0; i < mem_blocks->size(); i++){
        vmem->delete_block(get_block_name(smaller_bucket_name, bucket_num, i));
    }
    // Clean up disk_blocks
    vdisk->delete_relation(bucket1_name);
    vdisk->delete_relation(bucket2_name);
}

std::vector<tuple> Joiner::join_tuples(std::vector<tuple> r1_tuples, std::vector<tuple> r2_tuples, int r1_key_index, int r2_key_index){
    std::vector<tuple> new_tuples = {};
    // Hardcoding index for now
    for(auto r1_tuple : r1_tuples){
        for(auto r2_tuple : r2_tuples){
            if(r1_tuple[r1_key_index] == r2_tuple[r2_key_index]){
                new_tuples.push_back(tuple{r1_tuple[0], r1_tuple[1], r2_tuple[1]});
            }
        }
    }
    return new_tuples;
}

void Joiner::write_relations_to_disk(std::string relation1, std::vector<tuple>* r1_tuples, std::string relation2, std::vector<tuple>* r2_tuples){
    // Write r1_tuples to disk
    int num_blocks = r1_tuples->size() / TUPLES_PER_BLOCK + (r1_tuples->size() % TUPLES_PER_BLOCK != 0);
    for(int i = 0; i < num_blocks; i++){
        std::vector<tuple> tuples = {};
        for(int j = 0; j < TUPLES_PER_BLOCK; j++){
            if(i * TUPLES_PER_BLOCK + j >= r1_tuples->size())
                break;
            tuples.push_back(r1_tuples->at(i * TUPLES_PER_BLOCK + j));
        }
        vmem->insert_block(relation1, block(R1_KEY_INDEX, tuples));
        vmem->append_to_disk(vdisk, relation1);
        vmem->delete_block(relation1);
    }
    // Write r2_tuples to disk
    num_blocks = r2_tuples->size() / TUPLES_PER_BLOCK + (r2_tuples->size() % TUPLES_PER_BLOCK != 0);
    for(int i = 0; i < num_blocks; i++){
        std::vector<tuple> tuples = {};
        for(int j = 0; j < TUPLES_PER_BLOCK; j++){
            if(i * TUPLES_PER_BLOCK + j >= r2_tuples->size())
                break;
            tuples.push_back(r2_tuples->at(i * TUPLES_PER_BLOCK + j));
        }
        vmem->insert_block(relation2, block(R2_KEY_INDEX, tuples));
        vmem->append_to_disk(vdisk, relation2);
        vmem->delete_block(relation2);
    }
}

void Joiner::join(std::string relation1, std::string relation2, std::string output){
    int num_blocks = std::min(vdisk->get_relation_size(relation1), vdisk->get_relation_size(relation2));
    buckets = num_buckets(num_blocks);
    std::cout << "Num buckets: " << buckets << std::endl;
    hash_to_buckets(relation1, buckets);
    hash_to_buckets(relation2, buckets);
    merge_all_buckets(relation1, relation2, output);
}

int Joiner::get_key_index(std::string relation){
    if(relation.substr(0,2) == "R1")
        return R1_KEY_INDEX;
    else if(relation.substr(0,2) == "R2")
        return R2_KEY_INDEX;
    else
        return OUT_KEY_INDEX;
}

void Joiner::print(){
    vmem->print();
    vdisk->print();

}

void Joiner::print(std::string relation){
    vdisk->print(relation);
}

void Joiner::print(std::string relation, std::vector<int> keys){
    // Print the relation with the following keys
    auto blocks = vdisk->read_block_schedule(relation, 0);
    std::cout << "Printing select keys: " << std::endl;
    for(auto key : keys)
        std::cout << key << " ";
    std::cout << std::endl;
    for(auto block : blocks){
        for(auto tuple : block.tuples){
            int key_index = get_key_index(relation);
            for(auto key : keys) {
                if(tuple.at(key_index) == key){
                    for(auto val : tuple)
                        std::cout << val << " ";
                    std::cout << std::endl;
                }
            }
        }
    }
}
