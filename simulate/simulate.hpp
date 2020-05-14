#ifndef SIMULATE_HPP
#define SIMULATE_HPP

#include "../transmit/transmit.hpp"
#include "../data/tag_collection.hpp"
#include <iostream>

class config;
class tag_data;
class transmit;
class tag_collection;

class simulate {
    private:
        config &conf;
        transmit trans;
        tag_collection t_col;

        int establish_connection(bool status = true);
        int select_protocol(std::string protocol, bool status = true);
        void scan_for_tags(tag_data &t_data, bool random = false, int seed = 1, bool status = true);
    public:
        simulate(config &conf);
        void run();
};

#endif