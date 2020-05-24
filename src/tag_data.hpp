#ifndef TAGDATA_HPP
#define TAGDATA_HPP

#include <iostream>
#include <string>

class tag_data {
    public:
        int status;
        std::string status_code;
        std::string response_size;
        std::string tag_hex;
        std::string tag_binary;
        std::string crc;
        std::string protocol_error_status;

        void output_tag_summary(tag_data &data);
};

#endif