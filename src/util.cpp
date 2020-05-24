#include "util.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <regex>
#include <stdexcept>

std::string util::hex_to_bin(char hex[]) {
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

std::string util::reverse_tag(char hex[]) {
    int const len = strlen(hex);
    std::string reversed = "";

    for (int i = (len - 1); i > 0; i -= 2) {
        reversed += hex[i - 1];
        reversed += hex[i];
    }

    if (len % 2) {
        reversed += hex[0];
    }

    return reversed;
}

std::string util::identify_manufacturer(std::string protocol, char hex[]) {
    int const len = strlen(hex);
    std::string manufacturer = "";
    std::string identification_bits = "";

    if (len == 0) {
        throw std::invalid_argument("Invalid argument; non-existent tag.");
    }

    if (len < 4) {
        throw std::invalid_argument("Invalid argument; invalid tag format");
    }

    if (protocol == "15693") {
        identification_bits += hex[len - 4];
        identification_bits += hex[len - 3];

        if (identification_bits == "01") {
            manufacturer = "Motorola";
        } else if (identification_bits == "02") {
            manufacturer = "ST Microelectronics";
        } else if (identification_bits == "03") {
            manufacturer = "Hitachi";
        } else if (identification_bits == "04") {
            manufacturer = "NXP Semiconductors";
        } else if (identification_bits == "05") {
            manufacturer = "Infineon Technologies";
        } else if (identification_bits == "06") {
            manufacturer = "Cylinc";
        } else if (identification_bits == "07") {
            manufacturer = "Texas Intruments Tag-it";
        } else if (identification_bits == "08") {
            manufacturer = "Fujitsu Limited";
        } else if (identification_bits == "09") {
            manufacturer = "Matsushita Electric Industrial";
        } else if (identification_bits == "0A") {
            manufacturer = "NEC";
        } else if (identification_bits == "0B") {
            manufacturer = "Oki Electric";
        } else if (identification_bits == "0C") {
            manufacturer = "Toshiba";
        } else if (identification_bits == "0D") {
            manufacturer = "Mitsubishi Electric";
        } else if (identification_bits == "0E") {
            manufacturer = "Samsung Electronics";
        } else if (identification_bits == "0F") {
            manufacturer = "Hyundai Electronics";
        } else if (identification_bits == "10") {
            manufacturer = "LG Semiconductors";
        } else if (identification_bits == "16") {
            manufacturer = "EM Microelectronic-Marin";
        } else if (identification_bits == "1F") {
            manufacturer = "Melexis";
        } else if (identification_bits == "2B") {
            manufacturer = "Maxim";
        } else if (identification_bits == "33") {
            manufacturer = "AMIC";
        } else if (identification_bits == "44") {
            manufacturer = "Gentag, Inc (USA)";
        } else if (identification_bits == "45") {
            manufacturer = "Invengo Information Technology Co.Ltd";
        }
    }

    if (!manufacturer.size()) {
        throw std::invalid_argument("Invalid argument; invalid tag - unknown manufacturer namespace.");
    }

    return manufacturer;
}

void util::arr_slice(char arr[], char arr_part[], int begin, int end) {
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

    arr_part[++j] = '\0';
}

void util::arr_clear(char arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = '\0';
    }
}

void util::trim(std::string str, std::string &trimmed_str) {
    std::regex space("\\s");
    trimmed_str = "";

    for (int i = 0; i < (int)str.size(); i++) {
        if (!std::regex_match(str.substr(i, 1), space)) {
            trimmed_str.append(str.substr(i, 1));
        }
    }
}

void util::replace(std::string str, std::string &replaced_str, std::string pattern, std::string to_replace) {
   std::regex pat("^(.+)?" + pattern + "(.+)?$");
   std::string temp = str;

   if (pattern == "") {
       throw std::invalid_argument("Unable to replace nothing; illegal argument.");
   }

    while (std::regex_match(temp, pat)) {
        std::smatch match;
        std::regex_search(temp, match, pat);

        if ((int)match.size() > 1) {
            temp = match[1].str();
        }

        temp += to_replace;

        if ((int)match.size() > 2) {
            temp += match[2].str();
        }
    }

    replaced_str = temp;
}

void util::slice(std::string str, std::string &sliced_str, int begin, int end) {
    for (int i = begin; i < end; i++) {
        sliced_str += str[i];
    }
}

void util::split(std::string str, std::string ch, std::vector<std::string>* arr) {
    int begin = 0;

    for (int i = 0; i < (int)str.size(); i++) {
        if (str.substr(i, 1) == ch) {
            (*arr).push_back(str.substr(begin, i - begin));
            begin = i + 1;
        }
    }

    (*arr).push_back(str.substr(begin, (int)str.size() - begin));
}