#include"../include/common.hpp"
#include"../include/node.hpp"
#include<string>
#include<iostream>

int node_enumerator = 0;

Node::Node(int32_t order){
    std::cout<<"Node constructor called"<<std::endl;
    children.clear();
    values.clear();
    this->order = order;
    leaf = false;
    root = false;
    this->node_id = node_enumerator++;
}

Node::Node(int32_t order, bool leaf, bool root){
    std::cout<<"Node constructor called"<<std::endl;
    children.clear();
    values.clear();
    this->order = order;
    this->leaf = leaf;
    this->root = root;
    this->node_id = node_enumerator++;
}

Node::Node(int32_t order, bool leaf, bool root, std::vector<pop_frame> tuples){
    std::cout<<"Node constructor called"<<std::endl;
    assert((tuples.size() <= order+!leaf) && tuples.size() > 0);
    children.clear();
    values.clear();
    this->order = order;
    this->leaf = leaf;
    this->root = root;
    // For leaf nodes, we add all values and children; last tuple is the next node
    for (auto tuple : tuples) {
        children.push_back(tuple.node);
        if(tuple.value != EMPTY_INT)
            values.push_back(tuple.value);
    }
    this->node_id = node_enumerator++;
}

Node::~Node(){
    std::cout<<"Node destructor called"<<std::endl;
    if(is_leaf())
        return;
    for(auto child : children){
        if(child != EMPTY_PTR)
            delete child;
    }
}

std::string Node::to_string(){
    std::string result = "";
    if(is_root()){
        result += "R";
    } else if (is_leaf()) {
        result += "L";
    } else {
        result += "B";
    }
    result += std::to_string(node_id);
    // std::cout << "Node to string " << result << std::endl;
    return result;
}

bool Node::full(){
    if(is_leaf()){
        return values.size() == order;
    }
    return children.size() == order+1;
}

bool Node::empty(){
    return children.size() == 0;
}

bool Node::underflow(){
    return empty(); // Placeholder
}

bool Node::overflow(){
    if(is_leaf()){
        return values.size() > order;
    }
    return children.size() > order+1;
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
    assert(!overflow()); // The node must not be over full
    for (int32_t i = 0; i < values.size(); i++) {
        if (values[i] == value) {
            return i;
        }
    }
    std::cout << "Value " << value << " not found" << std::endl;
    return INT_BAD;
}

int32_t Node::search_child_index(int32_t value){
    assert(!is_leaf()); // The node must be a branch node
    assert(!overflow()); // The node must not be over full
    int32_t i = 0;
    std::cout << "Searching for " << value << std::endl;
    while (i < values.size() && values[i] < value) {
        i++;
    }
    std::cout << "Value size " << values.size() << std::endl;
    std::cout << "Children size " << children.size() << std::endl;
    assert(i >= 0 && i < children.size());
    if(i == values.size()){
        std::cout << "Value is bigger than all values; returning " << i << std::endl;
        return i;
    }
    std::cout << "Value " << value << " is smaller than " << values[i] << "; returning " << i << std::endl;
    return i;
}

void Node::add_value(int32_t value){
    assert(!full()); // The node must not be full
    assert(is_leaf()); // The node must be a leaf node
    int32_t i = 0;
    // Insert the new value in the correct position
    values.push_back(value);
    // sort
    std::sort(values.begin(), values.end());
}

bool Node::remove_value(int32_t value){
    assert(is_leaf());
    int32_t i = contains(value);
    if(i == INT_BAD){
        return BOOL_BAD;
    }
    values.erase(values.begin()+i);
    return BOOL_GOOD;
}

int32_t Node::get_num_children(){
    return children.size();
}

int32_t Node::get_num_values(){
    return values.size();
}

int32_t Node::get_min_value(){
    assert(!empty());
    if(is_leaf()){
        return values[0];
    }
    else{
        return children[0]->get_min_value();
    }
}

void Node::print(){
    if(is_root()){
        std::cout<<"Root " << node_id << ": ";
    } else if(is_leaf()){
        std::cout<<"Leaf " << node_id << ": ";
        for (auto value : values) {
            std::cout<<value<<" ";
        }
        std::cout<<std::endl;
    }
    else{
        std::cout<<"Branch " << node_id << ": ";
    }
    if(!is_leaf()){
        // Print the children and values
        for (int32_t i = 0; i < children.size(); i++) {
            std::cout << children[i]->to_string() << " ";
            if(i < values.size()){
                std::cout<<values[i]<<" ";
            }
        }
        std::cout << std::endl;
        for (auto child : children) {
            child->print();
        }
    }
    std::cout<<std::endl;
}

void Node::move_value(int32_t index, Node* dest){
    assert(!dest->full());
    dest->add_value(values[index]);
    values.erase(values.begin()+index);
}

/**
 * Push (MID_VALUE+1, LAST_VALUE) to the new node
 * Add the new tuple to the current node
 * Push the MID_VALUE+1 to the new node
 * Create the new node
 * Update the next node pointers for the new node and the current node
 * Return the new node in the pop_frame with the first value of the new node
*/
pop_frame Node::leaf_split(int32_t value){
    assert(full());
    std::cout << "Splitting leaf node" << std::endl;
    // std::cout << "Order is " << order << std::endl;
    // std::cout << "Pushing indexes " << order/2 << " to " << values.size() - 1 << std::endl;
    // Create a vector for the values
    std::vector<pop_frame> tuples;
    // We add the new tuple to the current node (allowing for overflow)
    values.push_back(value);
    std::sort(values.begin(), values.end());
    // Move half of the values to the new node
    for(int32_t i = values.size()-1; i > order/2; i--){
        tuples.insert(tuples.begin(), pop_frame(EMPTY_PTR, values.back()));
        values.pop_back();
        // std::cout << "Popping value " << tuples[0].value << std::endl;
    }
    // Create the new node
    Node* new_node = new Node(order, true, false, tuples);
    // Update the next node pointers for the new node and the current node
    new_node->next = next;
    next = new_node;
    return pop_frame(new_node, new_node->get_min_value());
}

pop_frame Node::branch_split(pop_frame new_tuple){
    assert(values.size() == order);
    assert(children.size() == order+1);
    std::cout << "Splitting branch node" << std::endl;
    // Create a vector for the values
    std::vector<pop_frame> tuples;
    // Insert the new tuple
    int32_t index = search_child_index(new_tuple.value);
    values.insert(values.begin()+index, new_tuple.value);
    children.insert(children.begin()+index+1, new_tuple.node);
    // Split the values and children
    for(int32_t i = values.size()-1; i > order/2; i--){
        tuples.insert(tuples.begin(), pop_frame(children.back(), values.back()));
        values.pop_back();
        children.pop_back();
    }
    int32_t pop_value = tuples[0].value;
    // Update the first value so it does not get written to the new node
    tuples[0].value = EMPTY_INT;
    // Create a new node
    Node* new_node = new Node(order, false, false, tuples);
    // Return the new node
    return pop_frame(new_node, pop_value);
}

pop_frame Node::search(int32_t value){
    if(!is_leaf()){
        // Search the correct child
        int32_t i = search_child_index(value);
        return children[i]->search(value);
    }
    // Check if the value is in the current node
    int32_t i = contains(value);
    // Leaf node does not have children
    if (i != INT_BAD) {
        return pop_frame(EMPTY_PTR, values[i]);
    }
    // Result not found
    return pop_frame();
}

std::vector<pop_frame> Node::get_range(int32_t end){
    assert(is_leaf()); // Range search is only supported in leaf nodes
    std::vector<pop_frame> results;
    print();
    // The range is in the current node
    int32_t i = 0;
    for(; i < values.size(); i++){
        // Check if we have reached the end of the range
        if(values[i] > end){
            return results;
        }
        results.push_back(pop_frame(children[i], values[i]));
    }
    // Check if we have reached the end of the leaf
    assert(i == values.size());
    if(!next) 
        return results;
    std::vector<pop_frame> tail = next->get_range(end);
    results.reserve( results.size() + tail.size() ); 
    results.insert( results.end(), tail.begin(), tail.end() );
    return results;
}

range_search_frame Node::leaf_range_search(int32_t start, int32_t end){
    assert(is_leaf()); // Range search is only supported in leaf nodes
    // The range is in the current node
    range_search_frame result_frame;
    print();
    int32_t i = 0;
    for(; i < values.size(); i++){
        // Get the first value in the range
        if(values[i] < start){
            continue;
        }
        // Check if the value is past the end of the range
        if(values[i] > end){
            return result_frame;
        }
        // Value is in the range
        result_frame.results.push_back(pop_frame(EMPTY_PTR, values[i]));
    }
    // Make sure we have reached the end of the leaf
    assert(i == values.size());
    if(!next){
        return result_frame;
    }
    std::vector<pop_frame> tail = next->get_range(end);
    if(tail.size() == 0) 
        return result_frame;
    result_frame.results.reserve( result_frame.results.size() + tail.size() ); 
    result_frame.results.insert( result_frame.results.end(), tail.begin(), tail.end() );
    return result_frame;
}


range_search_frame Node::range_search(int32_t start, int32_t end){
    // Check if the node is a branch
    if(!is_leaf()){
        // Search the correct child
        int32_t i = search_child_index(start);
        return children[i]->range_search(start, end);
    }
    return leaf_range_search(start, end);
}

pop_frame Node::branch_insert(pop_frame new_tuple){
    assert(!is_leaf()); // Branch insert is only supported in branch nodes
    // Search the correct child
    int32_t i = search_child_index(new_tuple.value);
    std::cout << "Inserting into child " << i << std::endl;
    pop_frame result = children[i]->insert(new_tuple);
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
        int32_t insert_index = search_child_index(result.value);
        // Check if the new node is the smallest value
        if(insert_index == 0){
            values.insert(values.begin(), children[0]->get_min_value());
            children.insert(children.begin(), result.node);
            return pop_frame();
        }
        // If new node is not the smallest value, insert the value and node
        values.insert(values.begin()+insert_index, result.value);
        children.insert(children.begin()+insert_index+1, result.node);
    }
    return pop_frame();

}

pop_frame Node::leaf_insert(pop_frame new_tuple){
    assert(is_leaf()); // Leaf insert is only supported in leaf nodes
    // Check if the value is already in the node
    int32_t i = contains(new_tuple.value);
    if(i != INT_BAD){
        std::cout << "Value already exists in the tree" << std::endl;
        return pop_frame();
    }
    // Check if the current node is full
    if(full()){
        std::cout << "Current node is full" << std::endl;
        // Split the current node
        return leaf_split(new_tuple.value);
    }
    // Add the new tuple to the current node
    add_value(new_tuple.value);
    return pop_frame();
}


pop_frame Node::insert(pop_frame new_tuple){
    // Check if the node is a branch
    if(!is_leaf()){
        return branch_insert(new_tuple);
    }
    // The node is a leaf
    return leaf_insert(new_tuple);
}

// Assume that the current node and the new node
pop_frame Node::branch_merge(pop_frame new_tuple){
    return pop_frame();
}

pop_frame Node::leaf_merge(pop_frame new_tuple){
    return pop_frame();
}

pop_frame Node::merge(pop_frame new_tuple){
    if(is_leaf()){
        return leaf_merge(new_tuple);
    }
    return branch_merge(new_tuple);
}

pop_frame Node::branch_remove(int32_t value){
    assert(!is_leaf());
    // Search the correct child
    int32_t i = search_child_index(value);
    pop_frame result = children[i]->remove(value);
    // Check if the child node was merged
    if(result.node != EMPTY_PTR){
        // Merge index i with neighbor
        if(i == 0){
            // Merge with the right neighbor
            result = children[i]->merge(pop_frame(children[i+1], EMPTY_INT));
            children.erase(children.begin() + i);
            values.erase(values.begin() + i);
            values[i] = children[i]->get_min_value();
        } else {
            // Merge with the left neighbor
            result = children[i - 1]->merge(result);
            children.erase(children.begin() + i);
            values.erase(values.begin() + i);
        }
        // Check if the current node is underfull
        if(underflow()){
            // Merge the current node
            return pop_frame(this, result.value);
        }
    }
    return pop_frame();
}

pop_frame Node::leaf_remove(int32_t value){
    assert(is_leaf());
    // Check if the value is in the node
    int32_t i = contains(value);
    if(i == INT_BAD){
        return pop_frame();
    }
    // Remove the tuple from the current node
    pop_frame result = pop_frame(EMPTY_PTR, values[i]);
    values.erase(values.begin() + i);
    // Check if the current node is underfull
    if(underflow()){
        // Merge the current node
        return pop_frame(this, result.value);
    }
    return result;
}

pop_frame Node::remove(int32_t value){
    // Check if the node is a branch
    if(!is_leaf()){
        return branch_remove(value);
    }
    return leaf_remove(value);
}

