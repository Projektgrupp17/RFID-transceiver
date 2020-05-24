#ifndef TAG_DEACTIVATOR_HPP
#define TAG_DEACTIVATOR_HPP

#include "../data/tag.hpp"
#include "../data/tag_data.hpp"
#include "../data/reader_device.hpp"
#include <thread>
#include <list>

class tag;
class transmitter;
class config;
class work_manager;

class tag_deactivator {
    private:
        std::thread thr;
        std::list<tag> queue;
        int interval;
        int sleep_duration;
        bool postponed;
        bool active;
        bool asleep;
        bool master;
        config &conf;
        transmitter &trs;
        work_manager &w_man;
        reader_device *r_dev;
    public:
        tag_deactivator(config &con, transmitter &trns, work_manager &w_ma, reader_device *r_de);
        void run();
        void close();
        void graceful_shutdown();
        void sleep(int ms);
        void postpone();
        void set_interval(int ms);
        void set_master(bool mode);
        void enqueue(tag &tg);
        bool is_active();
        bool is_asleep();
        static void deactivate(tag_deactivator *t_deact, config &conf, transmitter &trs, work_manager &w_man, reader_device *r_dev);
        static void shutdown(tag_deactivator *t_deact);
};

#endif