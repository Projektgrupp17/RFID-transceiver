#ifndef TAGDATA_HPP
#define TAGDATA_HPP
#include <string>

struct TagData {
    int status;
    std::string status_code;
    std::string response_size;
    std::string tag_hex;
    std::string tag_binary;
    std::string crc;
    std::string protocol_error_status;
};

#endif