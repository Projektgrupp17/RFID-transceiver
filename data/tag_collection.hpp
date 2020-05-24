#ifndef TAG_COLLECTION_HPP
#define TAG_COLLECTION_HPP

#include "tag_data.hpp"
#include "tag.hpp"
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>

class config;
class transmitter;
class tag_deactivator;

class tag_collection {
    private:
        std::list<std::pair<tag, long long int>> t_collection;
//        std::mutex mutex_t_collection;
        transmitter &trans;
        config &conf;
        std::pair<int, std::list<std::pair<tag, long long int>>::iterator> index_of(tag_data &data);

    public:
        tag_collection(config &con, transmitter &trs);
        tag_collection(config &con, transmitter &trs, tag_data &data, tag_deactivator &t_deact);
        void connect(tag_data &data, tag_deactivator &t_deact);
        void disconnect(tag &t);
        int time_to_live();
        void dump(tag_deactivator &t_deact);
        int size();
};

#endif