#include "tag.hpp"
#include "tag_data.hpp"
#include <chrono>

tag::tag(tag_data dat) : data(dat),  time_of_registration(std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1)) {

}

tag::tag(tag_data dat, int time) : data(dat), time_of_registration(time) {

}

tag_data tag::get_data() {
    return data;
}

int tag::get_time_of_registration() {
    return time_of_registration;
}