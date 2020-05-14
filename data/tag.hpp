#ifndef TAG_HPP
#define TAG_HPP

#include "tag_data.hpp"

class tag {
    private:
        tag_data data;
        int time_of_registration;
    public:
        tag(tag_data dat);
        tag(tag_data dat, int time);
        tag_data get_data();
        int get_time_of_registration();
};

#endif