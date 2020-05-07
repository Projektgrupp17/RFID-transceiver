#ifndef CONFIG_URL_HPP
#define CONFIG_URL_HPP

#include <string>

struct config_url {
    public:
        std::string protocol;
        std::string hostname;
        std::string port;
        std::string path;
        std::string original_url;
};

#endif