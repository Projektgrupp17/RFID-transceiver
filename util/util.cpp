#include <iostream>
#include <string>
#include <cstring>
#include <regex>

std::string hex_to_bin(char hex[]) {
    int const length = strlen(hex);
    std::string binary = "";

    for (int i = 0; i < length; i++) {
        if (hex[i] == '0') {
            binary += "0000";
        } else if (hex[i] == '1') {
            binary += "0001";
        } else if (hex[i] == '2') {
            binary += "0010";
        } else if (hex[i] == '3') {
            binary += "0011";
        } else if (hex[i] == '4') {
            binary += "0100";
        } else if (hex[i] == '5') {
            binary += "0101";
        } else if (hex[i] == '6') {
            binary += "0110";
        } else if (hex[i] == '7') {
            binary += "0111";
        } else if (hex[i] == '8') {
            binary += "1000";
        } else if (hex[i] == '9') {
            binary += "1001";
        } else if (hex[i] == 'A') {
            binary += "1010";
        } else if (hex[i] == 'B') {
            binary += "1011";
        } else if (hex[i] == 'C') {
            binary += "1100";
        } else if (hex[i] == 'D') {
            binary += "1101";
        } else if (hex[i] == 'E') {
            binary += "1110";
        } else if (hex[i] == 'F') {
            binary += "1111";
        } else {
            binary += "0000";
        }
    }

    return binary;
}

void arr_slice(char arr[], char arr_part[], int begin, int end) {
    int const length = strlen(arr);
    int j = 0;

    if (begin < 0 || begin >= length) {
        std::cerr << "Start index out of bounds.\n";
        return;
    }

    if (end < begin || end >= length) {
        std::cerr << "End index out of bounds or illegally set before begin index.\n";
        return;
    }

    if (end - begin <= 0) { //length_part...
        std::cout << "Nothing to do here. Moving along.\n";
        return;
    }

    for (int i = begin; i < end; i++, j++) {
        arr_part[j] = arr[i];
    }
}

void arr_clear(char arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = '\0';
    }
}

void trim(std::string str, std::string* trimmed_str) {
    std::regex space("\\s");
    *trimmed_str = "";

    for (int i = 0; i < (int)str.size(); i++) {
        if (!std::regex_match(str.substr(i, 1), space)) {
            (*trimmed_str).append(str.substr(i, 1));
        }
    }
}

void split(std::string str, std::string ch, std::vector<std::string>* arr) {
    int begin = 0;

    for (int i = 0; i < (int)str.size(); i++) {
        if (str.substr(i, 1) == ch) {
            (*arr).push_back(str.substr(begin, i - begin));
            begin = i + 1;
        }
    }

    (*arr).push_back(str.substr(begin, (int)str.size() - begin));
}