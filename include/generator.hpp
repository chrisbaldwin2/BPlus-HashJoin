#include "common.hpp"

class Generator {
public:
    Generator();
    ~Generator();
    void generate(int32_t num_nodes);
    std::vector<int32_t> get_nodes(int32_t num_nodes);
};
