#ifndef SIMULATE_HPP
#define SIMULATE_HPP
#include "../data/TagData.hpp"
#include <iostream>

int simulate_establish_connection(bool status = true);
int simulate_select_protocol(std::string protocol, bool status = true);
void simulate_scan_for_tags(TagData &tag_data, bool random = false, int seed = 1, bool status = true);

#endif