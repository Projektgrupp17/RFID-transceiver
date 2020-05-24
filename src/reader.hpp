#ifndef READER_HPP
#define READER_HPP

#include "reader_device.hpp"
#include "transmitter.hpp"
#include "tag_collection.hpp"
#include "tag_deactivator.hpp"
#include "work_manager.hpp"
#include <string>
#include <list>
#include <future>

class config;
class tag_data;

class reader : public reader_device {
    private:
        config &conf;
        transmitter trans;
        tag_collection t_col;
        tag_deactivator t_deact;
        work_manager w_man;
        bool deactivator_ready;

        int establish_connection();
        int select_protocol();
        void scan_for_tags(tag_data &tag_data);
    public:
        reader(config &con);
        void run();
        void toggle_deactivator_ready();
};

#endif