#include"../include/common.hpp"
#include"../include/node.hpp"
#include<string>
#include<iostream>


Node::Node(int32_t order){
    std::cout<<"Node constructor called"<<std::endl;
    children = new void*[order+1];
    values = new int32_t[order];
    for (int i = 0; i < NODE_VALUES_SIZE; i++) {
        values[i] = EMPTY_INT;
    }
    for (int i = 0; i < NODE_CHILDREN_SIZE; i++) {
        children[i] = EMPTY_PTR;
    }
    this->order = order;
    leaf = false;
    root = false;
}

Node::Node(int32_t order, bool leaf, bool root){
    children = new void*[order+1];
    values = new int32_t[order];
    for (int i = 0; i < NODE_VALUES_SIZE; i++) {
        values[i] = EMPTY_INT;
    }
    for (int i = 0; i < NODE_CHILDREN_SIZE; i++) {
        children[i] = EMPTY_PTR;
    }
    this->order = order;
    this->leaf = leaf;
    this->root = root;
}

Node::Node(int32_t order, bool leaf, bool root, std::vector<pop_frame> tuples){
    assert((tuples.size() <= order+!leaf) && tuples.size() > 0);
    children = new void*[order+1];
    values = new int32_t[order];
    this->order = order;
    this->leaf = leaf;
    this->root = root;
    // Default assignment
    for (int i = 0; i < NODE_VALUES_SIZE; i++) {
        values[i] = EMPTY_INT;
    }
    for (int i = 0; i < NODE_CHILDREN_SIZE; i++) {
        children[i] = EMPTY_PTR;
    }
    // For leaf nodes, we add all values and children; last tuple is the next node
    for (int i = 0; leaf && i < tuples.size()-1; i++) {
        values[i] = tuples[i].value;
        children[i] = tuples[i].node;
    }
    if (leaf) {
        children[NODE_CHILDREN_SIZE-1] = tuples[tuples.size()-1].node;
    } 
    // For branch nodes, we add all children and all values except the first one
    else {
        children[0] = tuples[0].node;
    }
    for (int i = 1; !leaf && i < tuples.size(); i++) {
        values[i-1] = tuples[i].value;
        children[i] = tuples[i].node;
    }
}

Node::~Node(){
    std::cout<<"Node destructor called"<<std::endl;
    for (int i = 0; i < NODE_CHILDREN_SIZE; i++) {
        if (children[i] != EMPTY_PTR && !is_leaf()) {
            delete((Node*) children[i]);
        }
        children[i] = EMPTY_PTR;
    }
    delete[] children;
    delete[] values;
}

bool Node::full(){
    if(is_leaf()){
        return (children[LAST_NODE_INDEX-1] != EMPTY_PTR);
    }
    return (children[LAST_NODE_INDEX] != EMPTY_PTR);
}

bool Node::empty(){
    return (children[0] == EMPTY_PTR);
}

bool Node::underflow(){
    return (children[0] == EMPTY_PTR);
}

bool Node::is_leaf(){
    return leaf;
}

bool Node::is_root(){
    return root;
}

void Node::set_root(bool root){
    this->root = root;
}

void Node::set_leaf(bool leaf){
    this->leaf = leaf;
}

int32_t Node::get_order(){
    return order;
}

int32_t Node::contains(int32_t value){
    for (int32_t i = 0; i < NODE_VALUES_SIZE; i++) {
        if (values[i] == value) {
            return i;
        }
    }
    return INT_BAD;
}

int32_t Node::search_child_index(int32_t value){
    assert(!is_leaf()); // The node must be a branch node
    int32_t i = 0;
    std::cout << "Searching for " << value << std::endl;
    while (i < NODE_VALUES_SIZE && values[i] < value) {
        if(values[i] == EMPTY_INT)
            break;
        i++;
    }
    std::cout << "Value " << value << " is smaller than " << values[i] << std::endl;
    return i;
}

void Node::add(pop_frame new_tuple){
    assert(!full()); // The node must not be full
    int32_t i = 0;
    // Insert the new value in the correct position
    while (i < NODE_VALUES_SIZE && values[i] != EMPTY_INT && values[i] < new_tuple.value) {
        i++;
    }
    // Shift the values to the right
    for (int j = NODE_VALUES_SIZE-1; j > i; j--) {
        values[j] = values[j-1];
        children[j] = children[j-1];
    }
    // Insert the new value
    values[i] = new_tuple.value;
    children[i] = new_tuple.node;
}

void Node::remove_value(int32_t value){
    assert(is_leaf());
    int32_t i = contains(value);
    if(i == INT_BAD){
        return;
    }
    // if(children[i] != EMPTY_PTR){
    //     delete(children[i]);
    // }
    // Shift the values to the left
    for (int j = i; j < NODE_VALUES_SIZE-1; j++) {
        values[j] = values[j+1];
        children[j] = children[j+1];
    }
    // Remove the last value
    values[NODE_VALUES_SIZE-1] = EMPTY_INT;
    children[NODE_CHILDREN_SIZE-1] = EMPTY_PTR;
}

int32_t Node::get_num_children(){
    int32_t num_children = 0;
    for (int i = 0; i < NODE_CHILDREN_SIZE; i++) {
        if (children[i] != EMPTY_PTR) {
            num_children++;
        }
    }
    return num_children;
}

int32_t Node::get_num_values(){
    int32_t num_values = 0;
    for (int i = 0; i < NODE_VALUES_SIZE; i++) {
        if (values[i] != EMPTY_INT) {
            num_values++;
        }
    }
    return num_values;
}

int32_t Node::get_min_value(){
    assert(!empty());
    if(is_leaf()){
        return values[0];
    }
    else{
        return ((Node*) children)[0].get_min_value();
    }
}

void Node::print(){
    std::cout<<"Node: ";
    for (int i = 0; i < NODE_VALUES_SIZE; i++) {
        if (values[i] != EMPTY_INT) {
            std::cout<<values[i]<<" ";
        }
    }
    std::cout<<std::endl;
}

void Node::move_value(int32_t index, Node* dest){
    assert(!dest->full());                // The destination node must not be full
    assert(dest != EMPTY_PTR);            // The destination node must be defined
    assert(values[index] != EMPTY_INT);   // The value being moved should be defined
    assert(index == NODE_CHILDREN_SIZE-1 || // is last value in branch
           (index == NODE_CHILDREN_SIZE-2 && is_leaf()) || // is last value in leaf
           values[index+1] == EMPTY_INT); // The value being moved should be the last value

    dest->add(pop_frame(children[index], values[index]));
    children[index] = EMPTY_PTR;
    values[index] = EMPTY_INT;
}

/**
 * Push (MID_VALUE+1, LAST_VALUE) to the new node
 * Add the new tuple to the current node
 * Push the MID_VALUE+1 to the new node
 * Create the new node
 * Update the next node pointers for the new node and the current node
 * Return the new node in the pop_frame with the first value of the new node
*/
pop_frame Node::leaf_split(pop_frame new_tuple){
    assert(full());
    std::cout << "Splitting leaf node" << std::endl;
    // Create a vector for the values
    std::vector<pop_frame> tuples;
    // Move half of the values except the middle value to the new node
    for(int32_t i = NODE_MID_INDEX+1; i < NODE_VALUES_SIZE; i++){
        tuples.push_back(pop_frame(children[i], values[i]));
        children[i] = EMPTY_PTR;
        values[i] = EMPTY_INT;
    }
    // Add the new tuple to the current node
    add(new_tuple);
    // Move the last value to the new node
    tuples.push_back(pop_frame(children[NODE_MID_INDEX+1], values[NODE_MID_INDEX+1]));
    children[NODE_MID_INDEX+1] = EMPTY_PTR;
    values[NODE_MID_INDEX+1] = EMPTY_INT;
    // Sort the tuples
    std::sort(tuples.begin(), tuples.end(), new_tuple.less_than);
    // Add the next leaf to the new node
    tuples.push_back(pop_frame(children[LAST_NODE_INDEX], EMPTY_INT));
    std::cout << "Tuples: ";
    for(auto t: tuples){
        std::cout << t.value << ", ";
    } std::cout << std::endl;
    // Create a new node
    Node* new_node = new Node(order, true, false, tuples);
    children[LAST_NODE_INDEX] = new_node;
    print();
    // new_node->print();
    int32_t pop_value = new_node->get_min_value();
    this->root = false;
    // Check that the size of the current node and the new node are correct
    assert(get_num_values() == NODE_MID_INDEX);
    assert(new_node->get_num_values() >= NODE_MID_INDEX && new_node->get_num_values() <= NODE_MID_INDEX+1);
    assert(get_num_children() == NODE_MID_INDEX+1);
    assert(new_node->get_num_children() >= NODE_MID_INDEX+1 && new_node->get_num_children() <= NODE_MID_INDEX+2);
    // Return the new node
    std::cout << "Leaf Split " << pop_value << std::endl;
    return pop_frame((void*) new_node, pop_value);
}


pop_frame Node::branch_split(pop_frame new_tuple){
    assert(full());
    std::cout << "Splitting branch node" << std::endl;
    // Create a vector for the values
    std::vector<pop_frame> tuples;
    // Move half of the values except the middle value to the new node
    for(int32_t i = NODE_MID_INDEX+1; i < NODE_VALUES_SIZE; i++){
        tuples.push_back(pop_frame(children[i], values[i]));
        children[i] = EMPTY_PTR;
        values[i] = EMPTY_INT;
    }
    // Add the new tuple to the current node
    add(new_tuple);
    // Move the last child to the new node
    tuples.push_back(pop_frame(children[NODE_MID_INDEX+1], values[NODE_MID_INDEX+1]));
    children[NODE_MID_INDEX+1] = EMPTY_PTR;
    values[NODE_MID_INDEX+1] = EMPTY_INT;
    // Sort the tuples
    std::sort(tuples.begin(), tuples.end(), new_tuple.less_than);
    // Create a new node
    Node* new_node = new Node(order, false, false, tuples);
    int32_t pop_value = values[NODE_MID_INDEX];
    values[NODE_MID_INDEX] = EMPTY_INT;
    // Check that the size of the current node and the new node are correct
    assert(get_num_values() == NODE_MID_INDEX-1);
    assert(new_node->get_num_values() >= NODE_MID_INDEX-1 && new_node->get_num_values() <= NODE_MID_INDEX);
    assert(get_num_children() == NODE_MID_INDEX+1);
    assert(new_node->get_num_children() >= NODE_MID_INDEX+1 && new_node->get_num_children() <= NODE_MID_INDEX+2);
    assert(new_node->get_min_value() == pop_value);
    // Return the new node
    return pop_frame((void*) new_node, pop_value);
}

pop_frame Node::search(int32_t value){
    // Check if the value is in the current node
    int32_t i = contains(value);
    if (is_leaf() && i != INT_BAD) {
        return pop_frame(children[i], values[i]);
    }
    // Check if the node is a leaf
    if (is_leaf()) {
        return pop_frame();
    }
    // Search the correct child
    i = search_child_index(value);
    return ((Node*) children)[i].search(value);
}

std::vector<pop_frame> Node::get_range(int32_t end){
    assert(is_leaf()); // Range search is only supported in leaf nodes
    std::vector<pop_frame> results;
    // The range is in the current node
    int32_t i = 0;
    for(; i < NODE_VALUES_SIZE; i++){
        // Check if we have reached the end of valid values in the leaf
        if(values[i] == EMPTY_INT){
            std::vector<pop_frame> tail = ((Node*) children)[LAST_NODE_INDEX].get_range(end);
            results.reserve( results.size() + tail.size() ); 
            results.insert( results.end(), tail.begin(), tail.end() );
            break;
        }
        // Check if we have reached the end of the range
        if(values[i] > end){
            break;
        }
        results.push_back(pop_frame(children[i], values[i]));
    }
    // Check if we have reached the end of the leaf
    if(i == NODE_VALUES_SIZE){
        std::vector<pop_frame> tail = ((Node*) children)[LAST_NODE_INDEX].get_range(end);
        results.reserve( results.size() + tail.size() ); 
        results.insert( results.end(), tail.begin(), tail.end() );
    }
    return results;
}

range_search_frame Node::range_search(int32_t start, int32_t end){
    // Check if the node is a branch
    if(!is_leaf()){
        // Search the correct child
        int32_t i = search_child_index(start);
        return ((Node*) children)[i].range_search(start, end);
    }
    // The node is a leaf
    // Check if the range is in the current node
    int32_t i = contains(start);
    if(i == INT_BAD){
        return range_search_frame();
    }
    // The range is in the current node
    range_search_frame result_frame;
    for(; i < NODE_VALUES_SIZE; i++){
        // Check if we have reached the end of valid values in the leaf
        if(values[i] == EMPTY_INT){
            std::vector<pop_frame> tail = ((Node*) children)[LAST_NODE_INDEX].get_range(end);
            result_frame.results.reserve( result_frame.results.size() + tail.size() ); 
            result_frame.results.insert( result_frame.results.end(), tail.begin(), tail.end() );
            break;
        }
        // Check if the value is past the end of the range
        if(values[i] > end){
            break;
        }
        result_frame.results.push_back(pop_frame(children[i], values[i]));
    }
    // Check if we have reached the end of the leaf
    if(i == NODE_VALUES_SIZE){
        std::vector<pop_frame> tail = ((Node*) children)[LAST_NODE_INDEX].get_range(end);
        result_frame.results.reserve( result_frame.results.size() + tail.size() ); 
        result_frame.results.insert( result_frame.results.end(), tail.begin(), tail.end() );
    }
    return result_frame;
}

pop_frame Node::insert(pop_frame new_tuple){
    // Check if the node is a branch
    if(!is_leaf()){
        // Search the correct child
        int32_t i = search_child_index(new_tuple.value);
        std::cout << "Inserting into child " << i << std::endl;
        pop_frame result = ((Node*) children)[i].insert(new_tuple);
        // Check if the child node was split
        if(result.node != EMPTY_PTR){
            std::cout << "Child node was split" << std::endl;
            // Check if the current node is full
            if(full()){
                std::cout << "Current node is full" << std::endl;
                // Split the current node
                return branch_split(result);
            }
            // Add the new node to the current node
            add(result);
        }
        return pop_frame();
    }
    // The node is a leaf
    // Check if the value is already in the node
    int32_t i = contains(new_tuple.value);
    if(i != INT_BAD){
        std::cout << "Value already exists in the tree" << std::endl;
        // Update the value
        children[i] = new_tuple.node;
        return pop_frame();
    }
    // Check if the current node is full
    if(full()){
        std::cout << "Current node is full" << std::endl;
        // Split the current node
        return leaf_split(new_tuple);
    }
    // Add the new tuple to the current node
    add(new_tuple);
    return pop_frame();
}

// Assume that the current node and the new node
pop_frame Node::branch_merge(pop_frame new_tuple){
    return pop_frame();
}

pop_frame Node::leaf_merge(pop_frame new_tuple){
    return pop_frame();
}

void Node::remove_child(void* child){
    // Search for the child
    int32_t i = 0;
    for(; i < NODE_VALUES_SIZE; i++){
        if(children[i] == child){
            break;
        }
    }
    delete (Node*) child;
    // Remove the child
    for(; i < NODE_CHILDREN_SIZE - 1; i++){
        children[i] = children[i + 1];
        values[i-1] = values[i];
    }
    children[NODE_CHILDREN_SIZE-1] = EMPTY_PTR;
    values[NODE_VALUES_SIZE-1] = EMPTY_INT;
}

pop_frame Node::remove(int32_t value){
    // Check if the node is a branch
    if(!is_leaf()){
        // Search the correct child
        int32_t i = search_child_index(value);
        pop_frame result = ((Node*) children)[i].remove(value);
        // Check if the child node was merged
        if(result.node != EMPTY_PTR){
            // Merge index i with neighbor
            if(i == 0){
                // Merge with the right neighbor
                result = branch_merge(result);
            } else {
                // Merge with the left neighbor
                result = ((Node*) children)[i - 1].branch_merge(result);
            }
            remove_child(result.node);
            // Check if the current node is underfull
            if(underflow()){
                // Merge the current node
                return pop_frame(this, result.value);
            }
        }
        return pop_frame();
    }

    // The node is a leaf
    // Check if the value is in the node
    int32_t i = contains(value);
    if(i == INT_BAD){
        return pop_frame();
    }
    // Remove the tuple from the current node
    pop_frame result = pop_frame(children[i], values[i]);
    // Check if the current node is underfull
    if(underflow()){
        // Merge the current node
        return leaf_merge(result);
    }
    return result;
}

