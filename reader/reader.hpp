#ifndef READER_HPP
#define READER_HPP

#include <string>

class config;
class tag_data;

class reader {
    public:
        void run(config &config);
    private:
        int establish_connection();
        int select_protocol(config &conf);
        void scan_for_tags(config &conf, tag_data &tag_data);
};

#endif