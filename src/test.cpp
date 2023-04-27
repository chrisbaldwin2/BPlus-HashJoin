#include "../include/test.hpp"
#include "../include/generator.hpp"
#include <string>
#include <random>
#include <iostream>

int basic_tree_test(){
    int32_t order = 5;
    BTree tree(order);
    tree.print();
    // Insert values into tree
    for(int i = 0; i < 21; i++){
        tree.insert(i);
        tree.print();
    }
    // Find value in tree
    bool found = tree.exists(10);
    if(found){
        std::cout << "Found 10" << std::endl;
    }
    else{
        std::cout << "Did not find 10" << std::endl;
    }
    // Value not in tree
    found = tree.exists(100);
    if(found){
        std::cout << "Found 100" << std::endl;
    }
    else{
        std::cout << "Did not find 100" << std::endl;
    }
    // Remove value from tree
    tree.remove(10);
    tree.print();
    // Remove value not in tree
    tree.remove(25);
    tree.print();
    // Range search
    range_search_frame result = tree.range_search(5, 15);
    std::cout << "Range search result: " << std::endl;
    for(int i = 0; i < result.results.size(); i++){
        std::cout << result.results[i].value << std::endl;
    }
    // Range search
    result = tree.range_search(10, 12);
    std::cout << "Range search result: " << std::endl;
    for(int i = 0; i < result.results.size(); i++){
        std::cout << result.results[i].value << std::endl;
    }
    return 0;
}

int basic_gen_test(){
    Generator gen;
    std::vector<int32_t>* values;
    gen.generate(10);
    values = gen.get_sequential_nodes(5);
    std::cout << "Sequential values: " << std::endl;
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        std::cout << *it << std::endl;
    }
    delete values;
    values = gen.get_random_nodes(5);
    std::cout << "Random values: " << std::endl;
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        std::cout << *it << std::endl;
    }
    gen.generate(10000);
    delete values;
    values = gen.get_sequential_nodes(100);
    std::cout << "Sequential values: " << values->size() << std::endl;
    delete values;
    return 0;
}

BTreeTest::BTreeTest(){
    this->order = 0;
    this->btree = NULL;
    this->generator = new Generator();
}

BTreeTest::~BTreeTest(){
    if(this->btree != NULL)
        delete this->btree;
    if(this->generator != NULL)
        delete this->generator;
}

void BTreeTest::set_order(int32_t order){
    this->order = order;
}

void BTreeTest::generate_nodes(int32_t num_nodes){
    this->generator->generate(num_nodes);
}

void BTreeTest::clean_tree(){
    if(this->btree != NULL)
        delete this->btree;
    this->btree = NULL;
}


/**
 * Even order: split_index = order/2 + 1
 * - 6 : 0, 1, 2, 3, 4, 5
 *   6/2 + 1 = 4
 *   0, 1, 2, 3 | 4, 5, 6
 * Odd order: split_index = order/2 + 1
 * - 7 : 0, 1, 2, 3, 4, 5, 6
 *   7/2 + 1 = 4
 *   0, 1, 2, 3 | 4, 5, 6, 7
 * 
 * 0-3 are inserted first, skipping split_offset values
 * the rest of values are inserted with the same pattern
 * 
 * EX: order = 6
 * 0: insert { % = 0}
 * 1: insert { % = 1}
 * 2: insert { % = 2}
 * 3: insert { % = 3}
 * 4: +split_offset = 2 = order - split_index = 6 - 4 { % = 4; split_index}
 * 6: insert { % = 0}
 * 7: insert { % = 1}
 * 8: insert { % = 2}
 * 9: insert { % = 3}
 * 10: +split_offset { % = 4; split_index}
 * 
 * 4: insert { % = 4; split_index}
 * 5: insert { % = 5}
 * 6: +split_index = 4 { % = 0}
 * 10: insert { % = 4; split_index}
 * 11: insert { % = 5}
 * 12: +split_index = 4 { % = 0}
 * 
*/
void BTreeTest::build_dense_tree(){
    this->btree = new BTree(this->order);
    std::vector<int32_t>* values = this->generator->get_nodes();
    // The first value after the split
    const int split_index = order / 2 + 1; 
    const int split_offset = order - split_index;
    // Insert every other value
    for(int i = 0; i < values->size();){
        if(i%order == split_index){
            i+=split_offset;
            assert(i%order == 0);
            continue;
        }
        this->btree->insert(values->at(i)); 
        i++;
    }
    // Insert the rest of the values
    for(int i = split_index; i < values->size();){
        if(i%order == 0){
            i+=split_index;
            assert(i%order == split_index);
            continue;
        }
        this->btree->insert(values->at(i)); 
        i++;
    }
    this->btree->print();
    delete values;
}

void BTreeTest::build_sparse_tree(){
    // Build a sparse tree
    this->btree = new BTree(order);
    std::cout << "Getting sequential nodes" << std::endl;
    std::vector<int32_t>* values = this->generator->get_nodes();
    std::cout << "Iterating nodes nodes" << std::endl;
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        std::cout << "Inserting: " << *it << std::endl;
        this->btree->insert(*it);
    }
    this->btree->print();
    std::cout << "Done building sparse tree" << std::endl;
    delete values;
}

void BTreeTest::insert_random(int32_t num_values){
    // Insert random values by using the generated nodes
    // and inserting between them

    std::vector<int32_t>* values = this->generator->get_outer_nodes(num_values);
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        this->btree->insert(*it);
        std::cout << "Inserted: " << *it << std::endl;
    }
    delete values;
}

void BTreeTest::delete_random(int32_t num_values){
    // Delete random values by using the generated nodes

    std::vector<int32_t>* values = this->generator->get_random_nodes(num_values);
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        this->btree->remove(*it);
    }
    delete values;
}

void BTreeTest::search_random(int32_t num_values){
    // Search random values by using the generated nodes

    std::vector<int32_t>* values = this->generator->get_random_nodes(num_values);
    for(std::vector<int32_t>::iterator it = values->begin(); it != values->end(); it++){
        this->btree->search(*it);
    }
    delete values;
}

void BTreeTest::range_search_random(int32_t num_values) {
    // Search random values by using the generated nodes

    for(int i = 0; i < num_values; i++) {
        std::vector<int32_t>* values = this->generator->get_random_nodes(2);
        std::sort(values->begin(), values->end());
        this->btree->range_search(values->at(0), values->at(1));
        delete values;
    }
}

void BTreeTest::random_random(int32_t num_values){
    // Randomly insert or delete values

    for(int i = 0; i < num_values; i++) {
        // Generate a random bool
        bool insert = rand() % 2;
        if(insert){
            insert_random(1);
        } else {
            delete_random(1);
        }
    }
}

bool BTreeTest::test_sparse_tree(){
    generate_nodes(10000);
    // generator->print();
    build_sparse_tree();
    std::cout << "Inserting random values" << std::endl;
    insert_random(2);
    std::cout << "Deleting random values" << std::endl;
    delete_random(2);
    std::cout << "Searching random values" << std::endl;
    search_random(5);
    std::cout << "Range searching random values" << std::endl;
    range_search_random(5);
    std::cout << "Returning sparse tree test" << std::endl;
    return true;
}

bool BTreeTest::test_dense_tree(){
    generate_nodes(10000);
    build_dense_tree();
    std::cout << "Inserting random values" << std::endl;
    insert_random(2);
    std::cout << "Deleting random values" << std::endl;
    delete_random(2);
    std::cout << "Searching random values" << std::endl;
    search_random(5);
    std::cout << "Range searching random values" << std::endl;
    range_search_random(5);
    std::cout << "Returning dense tree test" << std::endl;
    return true;
}

bool BTreeTest::run(){
    std::cout << "Running all tests" << std::endl;
    bool result = BOOL_GOOD;
    result &= run_1();
    result &= run_2();
    result &= run_3();
    result &= run_4();
    return result;
}

bool BTreeTest::run_1(){
    std::cout << "~~~~~~~~~Running btree1~~~~~~~~~" << std::endl << std::endl;
    // Build order 13 tree
    this->order = 13;
    std::cout << "Testing order 13 sparse tree" << std::endl;
    test_sparse_tree();
    clean_tree();
    return true;
}

bool BTreeTest::run_2(){
    std::cout << "~~~~~~~~~Running btree2~~~~~~~~~" << std::endl << std::endl;
    // Build order 13 tree
    this->order = 13;
    std::cout << "Testing order 13 sparse tree" << std::endl;
    test_dense_tree();
    clean_tree();
    return true;
}

bool BTreeTest::run_3(){
    std::cout << "~~~~~~~~~Running btree3~~~~~~~~~" << std::endl << std::endl;
    // Build order 24 tree
    this->order = 24;
    std::cout << "Testing order 24 sparse tree" << std::endl;
    test_sparse_tree();
    clean_tree();
    return true;
}

bool BTreeTest::run_4(){
    std::cout << "~~~~~~~~~Running btree4~~~~~~~~~" << std::endl << std::endl;
    // Build order 24 tree
    this->order = 24;
    std::cout << "Testing order 24 sparse tree" << std::endl;
    test_dense_tree();
    clean_tree();
    return true;
}

HashTest::HashTest(){
    this->joiner = NULL;
    // Build VMem and VDisk
    this->vdisk = new VDisk();
    this->vmem = new VMem(15);
    // Build generator
    this->generator = new TupleGenerator();
    // Set relation names
    this->r1_name = "R1";
    this->r2_name = "R2";
    this->output_name = "OUT";
}

HashTest::~HashTest(){
    if(this->joiner != NULL)
        delete this->joiner;
    if(this->generator != NULL)
        delete this->generator;
    if(this->vmem != NULL)
        delete this->vmem;
    if(this->vdisk != NULL)
        delete this->vdisk;
}

void HashTest::clean_joiner(){
    // Clean joiner
    if(this->joiner != NULL)
        delete this->joiner;
    this->joiner = NULL;
    this->vmem->clean();
    this->vdisk->clean();
}

void HashTest::build_joiner(){
    // Build joiner
    this->joiner = new Joiner(vmem, vdisk);
}

void HashTest::write_tuples_to_disk(){
    // Assume tuples are generated
    auto r1_tuples = generator->get_r1_tuples();
    auto r2_tuples = generator->get_r2_tuples();
    // Write tuples to disk
    joiner->write_relations_to_disk(r1_name, r1_tuples, r2_name, r2_tuples);
}

void HashTest::print(){
    // Print
    joiner->print();
}

bool HashTest::run_5_1(){
    std::cout << "~~~~~~~~~Running hash1~~~~~~~~~" << std::endl << std::endl;
    // Generate tuples
    std::cout << "Generating tuples" << std::endl;
    generator->generate_5_1();
    // Build joiner
    std::cout << "Building joiner" << std::endl;
    build_joiner();
    // Write tuples to disk
    std::cout << "Writing tuples to disk" << std::endl;
    write_tuples_to_disk();
    // print();
    // Join
    std::cout << "Joining" << std::endl;
    joiner->join(r1_name, r2_name, output_name);
    // Output Disk Ops
    std::cout << "Disk Ops: " << vdisk->get_num_ops() << std::endl;
    // Print
    std::cout << "Printing" << std::endl;
    std::vector<int32_t>* keys = generator->get_keys(20);
    joiner->print(output_name, *keys);
    delete keys;
    // Clean joiner
    std::cout << "Cleaning" << std::endl;
    clean_joiner();
    return true;
}

bool HashTest::run_5_2(){
    std::cout << "~~~~~~~~~Running hash2~~~~~~~~~" << std::endl << std::endl;
    // Generate tuples
    std::cout << "Generating tuples" << std::endl;
    generator->generate_5_2();
    // Build joiner
    std::cout << "Building joiner" << std::endl;
    build_joiner();
    // Write tuples to disk
    std::cout << "Writing tuples to disk" << std::endl;
    write_tuples_to_disk();
    // print();
    // Join
    std::cout << "Joining" << std::endl;
    joiner->join(r1_name, r2_name, output_name);
    // Output Disk Ops
    std::cout << "Disk Ops: " << vdisk->get_num_ops() << std::endl;
    // Print
    std::cout << "Printing" << std::endl;
    joiner->print(output_name);
    // Clean joiner
    std::cout << "Cleaning" << std::endl;
    clean_joiner();
    return true;
}

bool HashTest::run(){
    std::cout << "Running all tests" << std::endl;
    bool result = BOOL_GOOD;
    result &= run_5_1();
    result &= run_5_2();
    return result;
}