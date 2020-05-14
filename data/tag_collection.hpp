#ifndef TAG_COLLECTION_HPP
#define TAG_COLLECTION_HPP

#include "tag_data.hpp"
#include "tag.hpp"
#include "tag_deactivator.hpp"
#include <iostream>
#include <list>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>

class config;
class transmit;

class tag_collection {
    private:
    //    std::list<std::pair<tag, std::future<std::pair<std::string, bool>>>> t_collection;
        std::list<std::pair<tag, long long int>> t_collection;
//        std::mutex mutex_t_collection;
        transmit &trans;
        config &conf;
        tag_deactivator t_deact;
        std::list<std::pair<std::string, bool>> msg;

        std::pair<int, std::list<std::pair<tag, long long int>>::iterator> index_of(tag_data &data);
        void handle_msg();
        void handle_work_delegation();
    public:
        tag_collection(transmit &trs, config &con);
        tag_collection(tag_data &data, transmit &trs, config &con);
        void connect(tag_data &data);
        void store_msg(std::pair<std::string, bool> mesg);
        void dump();
        int size();
};

#endif