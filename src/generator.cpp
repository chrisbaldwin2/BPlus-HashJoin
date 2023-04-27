#include "../include/generator.hpp"
#include <random>

Generator::Generator(){
    nodes.clear();
}

Generator::~Generator(){
    nodes.clear();
}

void Generator::print(){
    std::cout << "Nodes: " << std::endl;
    for(std::set<int32_t>::iterator it = nodes.begin(); it != nodes.end(); it++){
        std::cout << *it << std::endl;
    }
}

void Generator::generate(int32_t num_nodes){
    nodes.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(1000000, 2000000);
    while(nodes.size() < num_nodes){
        nodes.insert(dis(gen));
        // print();
    }
}

std::vector<int32_t>* Generator::get_random_nodes(int32_t num_nodes){
    int32_t index = rand() % nodes.size();
    std::set<int32_t> set_result;
    std::set<int32_t>::iterator it = nodes.begin();
    std::advance(it, index);
    while(set_result.size() < num_nodes){
        set_result.insert(*it);
        index = rand() % nodes.size();
        it = nodes.begin();
        std::advance(it, index);
    }
    std::vector<int32_t>* out = new std::vector<int32_t>();
    for(int i = 0; i < num_nodes; i++){
        // Pick a random node from the set
        index = rand() % set_result.size();
        it = set_result.begin();
        std::advance(it, index);
        out->push_back(*it);
        set_result.erase(it);
    }
    return out;
};

std::vector<int32_t>* Generator::get_sequential_nodes(int32_t num_nodes){
    int32_t start_index = rand() % (nodes.size() - num_nodes);
    std::vector<int32_t>* result = new std::vector<int32_t>();
    std::set<int32_t>::iterator it = nodes.begin();
    std::advance(it, start_index);
    for(int i = 0; i < num_nodes; i++){
        result->push_back(*it);
        it++;
    }
    return result;
};

/** Go to a random node, then find the next node. Calculate the distance between the two nodes.
 * If the distance is 1, then the nodes are sequential. If the distance is greater than 1, then
 * the nodes are not sequential. Pick a random node between the two nodes, and repeat the process.
 * Once at the end of the tree, go back to the beginning and repeat the process until the desired
 * number of nodes have been found.
 */

std::vector<int32_t>* Generator::get_outer_nodes(int32_t num_nodes){
    std::vector<int32_t>* result = new std::vector<int32_t>();
    while(result->size() < num_nodes){
        std::set<int32_t>::iterator it = nodes.begin();
        int32_t start_index = rand() % (nodes.size());
        std::advance(it, start_index);
        int32_t prev = *it;
        int32_t next = *it;
        int32_t distance = 0;
        // Find the next node
        while(distance <= 1){
            prev = next;
            next = *it;
            distance = next - prev;
            // std::cout << "Prev: " << prev << " Next: " << next << " Distance: " << distance << std::endl;
            // std::cout << "Result size: " << result->size() << std::endl;
            it++;
            if(it == nodes.end()){
                it = nodes.begin();
                next = *it;
            }
        }
        // Pick a random node between the two nodes
        int32_t index = (rand() % distance-1) + 1;
        int32_t node = prev + index;
        // std::cout << "Prev: " << prev << " Next: " << next << " New Node: " << node << std::endl;
        result->push_back(node);
        // std::cout << "Result size: " << result->size() << std::endl;
    }
    // std::cout << "Result size: " << result->size() << " num_nodes " << num_nodes << std::endl;
    return result;
};

std::vector<int32_t>* Generator::get_nodes(){
    std::vector<int32_t>* result = new std::vector<int32_t>();
    for(std::set<int32_t>::iterator it = nodes.begin(); it != nodes.end(); it++){
        result->push_back(*it);
    }
    return result;
};

TupleGenerator::TupleGenerator(){
    r1_tuples.clear();
    r2_tuples.clear();
}

TupleGenerator::~TupleGenerator(){
    r1_tuples.clear();
    r2_tuples.clear();
}

void TupleGenerator::print(){
    std::cout << "R1 Tuples: " << std::endl;
    for(std::vector<tuple>::iterator it = r1_tuples.begin(); it != r1_tuples.end(); it++){
        std::cout << it->at(0) << " " << it->at(1) << std::endl;
    }
    std::cout << "R2 Tuples: " << std::endl;
    for(std::vector<tuple>::iterator it = r2_tuples.begin(); it != r2_tuples.end(); it++){
        std::cout << it->at(0) << " " << it->at(1) << std::endl;
    }
}

/**
 * Generate a relation S(B,C) with 5,000 tuples, where B is a random integer between 10,000 and 50,000
 * and C is a random integer between 1 and 100. B is the primary key.
 * 
 * Generate a relation R(A,B) with 1,000 tuples, where B is a random key from S and A is a random.
*/
void TupleGenerator::generate(int32_t r1_tuples, int32_t r2_tuples, bool use_r1_keys){
    std::set<tuple> tuples;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis1(1, 100);
    std::uniform_int_distribution<int32_t> dis2(10000, 50000);

    std::cout << "Generating set" << std::endl;
    while(tuples.size() < r1_tuples){
        tuple t;
        t.push_back(dis1(gen));
        t.push_back(dis2(gen));
        tuples.insert(t);
    }

    std::cout << "Copying set to vector" << std::endl;
    for(std::set<tuple>::iterator it = tuples.begin(); it != tuples.end(); it++){
        this->r1_tuples.push_back(*it);
    }
    std::cout << "R1 Tuples: " << this->r1_tuples.size() << std::endl;
    tuples.clear();

    std::uniform_int_distribution<int32_t> dis3;
    if(use_r1_keys)
        dis3 = std::uniform_int_distribution<int32_t>(0, r1_tuples-1);
    else
        dis3 = std::uniform_int_distribution<int32_t>(20000, 30000);
    std::uniform_int_distribution<int32_t> dis4(1000, 10000);
    std::cout << "Generating set" << std::endl;
    while(tuples.size() < r2_tuples){
        tuple t;
        if(use_r1_keys)
            t.push_back(this->r1_tuples.at(dis3(gen)).at(R1_KEY_INDEX));
        else
            t.push_back(dis3(gen));
        t.push_back(dis4(gen));
        tuples.insert(t);
    }
    std::cout << "Copying set to vector" << std::endl;
    for(std::set<tuple>::iterator it = tuples.begin(); it != tuples.end(); it++){
        this->r2_tuples.push_back(*it);
    }
    std::cout << "R2 Tuples: " << this->r2_tuples.size() << std::endl;
}


void TupleGenerator::generate_5_1(){
    generate(1000, 5000, true);
}

void TupleGenerator::generate_5_2(){
    generate(1200, 5000, false);
}

std::vector<tuple>* TupleGenerator::get_r1_tuples(){
    return &r1_tuples;
}

std::vector<tuple>* TupleGenerator::get_r2_tuples(){
    return &r2_tuples;
}

std::vector<int32_t>* TupleGenerator::get_keys(int num_keys){
    std::set<int32_t> keys;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dis(0, r1_tuples.size()-1);
    while(keys.size() < num_keys){
        int32_t key = r1_tuples.at(dis(gen)).at(R1_KEY_INDEX);
        keys.insert(key);
    }
    std::vector<int32_t>* result = new std::vector<int32_t>();
    for(std::set<int32_t>::iterator it = keys.begin(); it != keys.end(); it++){
        result->push_back(*it);
    }
    return result;
}
