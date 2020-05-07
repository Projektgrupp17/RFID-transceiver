#ifndef SIMULATE_HPP
#define SIMULATE_HPP

#include <iostream>

class config;
class tag_data;

class simulate {
    public:
        void run(config &conf);
    private:
        int establish_connection(bool status = true);
        int select_protocol(std::string protocol, bool status = true);
        void scan_for_tags(tag_data &t_data, bool random = false, int seed = 1, bool status = true);
};

#endif