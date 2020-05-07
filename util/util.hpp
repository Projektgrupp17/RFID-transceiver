#ifndef UTIL_HPP
#define UTIL_HPP
#include <string>
#include <vector>
#include <cstring>

class util {
    public:
        std::string static hex_to_bin(char hex[]);
        std::string static reverse_tag(char hex[]);
        std::string static identify_manufacturer(std::string protocol, char hex[]);
        void static arr_slice(char arr[], char arr_part[], int begin, int end);
        void static arr_clear(char arr[], int size);
        void static trim(std::string str, std::string &trimmed_str);
        void static replace(std::string str, std::string &replaced_str, std::string pattern, std::string to_replace = "");
        void static slice(std::string str, std::string &sliced_str, int begin, int end);
        void static split(std::string str, std::string ch, std::vector<std::string>* arr);
};

#endif