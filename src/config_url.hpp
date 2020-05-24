#ifndef CONFIG_URL_HPP
#define CONFIG_URL_HPP

#include <string>

struct config_url {
    public:
        std::string reg_protocol;
        std::string reg_hostname;
        std::string reg_port;
        std::string reg_path;
        std::string reg_original_url;
        std::string unreg_protocol;
        std::string unreg_hostname;
        std::string unreg_port;
        std::string unreg_path;
        std::string unreg_original_url;
};

#endif