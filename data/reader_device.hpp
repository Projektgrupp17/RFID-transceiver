#ifndef READER_DEVICE_HPP
#define READER_DEVICE_HPP

#include <string>
#include <utility>

class config;
class tag_data;

class reader_device {
    public:
        reader_device() {};
        virtual ~reader_device() {};
        virtual void run() = 0;
        virtual void toggle_deactivator_ready() {deactivator_ready = !deactivator_ready;};
        
    private:
        bool deactivator_ready = false;

        virtual int establish_connection() = 0;
        virtual int select_protocol() = 0;
        virtual void scan_for_tags(tag_data &tag_data) = 0;
};

#endif