#include "tag_data.hpp"

void tag_data::output_tag_summary(tag_data &data) {
    std::cout << "Tag uid: 0x" << data.tag_hex
        << "\nTag uid binary: " << data.tag_binary << "\nCRC: " << data.crc << "\n";
}