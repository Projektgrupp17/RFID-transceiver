#ifndef TAG_DEACTIVATOR_HPP
#define TAG_DEACTIVATOR_HPP

#include "tag.hpp"
#include "tag_data.hpp"
#include <thread>
#include <list>

class tag;
class tag_collection;
class transmit;
class config;

class tag_deactivator {
    private:
        std::thread thr;
        std::list<tag> queue;
        int interval;
        int sleep_duration;
        bool postponed;
        bool active;
        bool asleep;
        tag_data data;
        config &conf;
        transmit &trs;
        tag_collection *t_col;
    public:
        tag_deactivator(transmit &trns, config &con, tag_collection *t_co);
        void run();
        void close();
        void graceful_shutdown();
        void sleep(int ms);
        void postpone();
        void set_interval(int ms);
        void enqueue(tag &tg);
        bool is_active();
        bool is_asleep();
        static void deactivate(tag_deactivator *t_deact, tag_collection *t_col, transmit trs, config conf);
        static void shutdown(tag_deactivator *t_deact);
};

#endif