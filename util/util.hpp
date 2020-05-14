#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
#include <cstring>

class util {
    public:
        static std::string hex_to_bin(char hex[]);
        static std::string reverse_tag(char hex[]);
        static std::string identify_manufacturer(std::string protocol, char hex[]);
        static void arr_slice(char arr[], char arr_part[], int begin, int end);
        static void arr_clear(char arr[], int size);
        static void trim(std::string str, std::string &trimmed_str);
        static void replace(std::string str, std::string &replaced_str, std::string pattern, std::string to_replace = "");
        static void slice(std::string str, std::string &sliced_str, int begin, int end);
        static void split(std::string str, std::string ch, std::vector<std::string>* arr);
};

#endif