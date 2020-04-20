#ifndef READER_HPP
#define READER_HPP
#include "../data/TagData.hpp"
#include <string>

int establish_connection();
int select_protocol(std::string protocol);
void scan_for_tags(TagData tag_data);

#endif