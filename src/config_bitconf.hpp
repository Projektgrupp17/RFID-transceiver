#ifndef CONFIG_BITCONF_HPP
#define CONFIG_BITCONF_HPP

#include <string>

struct config_bitconf {
    public:
        std::string select_protocol;
        std::string select_parameters;
        std::string send_data;
};

#endif