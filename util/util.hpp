#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
#include <cstring>

std::string hex_to_bin(char hex[]);
void arr_slice(char arr[], char arr_part[], int begin, int end);
void arr_clear(char arr[], int size);
void trim(std::string str, std::string* trimmed_str);
void split(std::string str, std::string ch, std::vector<std::string>* arr);

#endif