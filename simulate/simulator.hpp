#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "../data/reader_device.hpp"
#include "../transmit/transmitter.hpp"
#include "../data/tag_collection.hpp"
#include "../tag_deactivator/tag_deactivator.hpp"
#include "../data/work_manager.hpp"
#include <iostream>

class config;
class tag_data;

class simulator : public reader_device {
    private:
        config &conf;
        transmitter trans;
        tag_collection t_col;
        tag_deactivator t_deact;
        work_manager w_man;
        bool deactivator_ready;
        int seed_offset;

        int establish_connection(); // bool status = true
        int select_protocol(); // std::string protocol, bool status = true
        void scan_for_tags(tag_data &tag_data);
        void scan_for_tags(tag_data &t_data, bool random = false, bool status = true);
    public:
        simulator(config &conf);
        void run();
        void toggle_deactivator_ready();
};

#endif