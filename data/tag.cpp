#include "../data/tag.hpp"
#include "../data/tag_data.hpp"
#include <chrono>

tag::tag(tag_data dat) :  time_of_registration(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {
data = dat;
}

tag::tag(tag_data dat, int time) : time_of_registration(time) {
data = dat;
}

tag_data tag::get_data() {
    return data;
}

int tag::get_time_of_registration() {
    return time_of_registration;
}