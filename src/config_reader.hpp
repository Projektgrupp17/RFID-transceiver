#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <string>
#include <vector>

struct config_reader {
    public:
        std::string protocol;
        std::vector<std::string> rotate;
        bool rotate_protocol;
        std::string scan_delay;
        std::string time_to_live;
        std::string repeat;
};

#endif