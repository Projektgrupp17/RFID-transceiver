#include "tag_collection.hpp"
#include "tag_data.hpp"
#include "tag.hpp"
#include "transmitter.hpp"
#include "config.hpp"
#include "tag_deactivator.hpp"
#include <thread>

tag_collection::tag_collection(config &con, transmitter &trs) : trans(trs), conf(con) {

};

tag_collection::tag_collection(config &con, transmitter &trs, tag_data &data, tag_deactivator &t_deact) : trans(trs), conf(con) {
    connect(data, t_deact);
}

void tag_collection::connect(tag_data &data, tag_deactivator &t_deact) {
    auto index = index_of(data);
    tag t = tag(data);
    long long int time_to_live = (std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1)) + std::stoi(conf.get_time_to_live());
    int sleep = std::stoi(conf.get_time_to_live());
/*    std::unique_lock<std::mutex> lock_t_collection(mutex_t_collection, std::defer_lock);
    while (!lock_t_collection.try_lock()) {} */

    if (index.first != -1) {
        int register_time = index.second->first.get_time_of_registration();
        t = tag(data, register_time);

        if (index.second == t_collection.begin()) {
            auto next = index.second;

            if (++next != t_collection.end()) {
                int first = index.second->second;
                int second = next->second;
                sleep = second - first;
            } else {
                long long int time_now = std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
                sleep = (time_now + std::stoi(conf.get_time_to_live())) - index.second->second;
            }

            if (t_deact.is_asleep()) {
                t_deact.postpone();
            }

            t_deact.sleep(sleep); 
        }

        t_collection.erase(index.second);
    } else {
        if (!t_deact.is_active()) {
            t_deact.run();
            t_deact.sleep(sleep);
        }
    }

    std::pair<tag, long long int> pair = std::make_pair(t, time_to_live);
    t_collection.push_back(pair);
//    lock_t_collection.unlock();
}

std::pair<int, std::list<std::pair<tag, long long int>>::iterator> tag_collection::index_of(tag_data &data) {
    int index = -1;
    auto iter = t_collection.begin();
    
    for (int i = 0; iter != t_collection.end(); iter++, i++) {
        if (iter->first.get_data().tag_hex == data.tag_hex) {
            index = i;
            break;
        }
    }

    auto res = std::make_pair(index, iter);

    return res;
}

void tag_collection::disconnect(tag &t) {
    auto iter = t_collection.begin();
    t = iter->first;
    t_collection.pop_front();
}

int tag_collection::time_to_live() {
    auto iter = t_collection.begin();
    long long int now = std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    long long int ttl = iter->second - now;

    return ttl;
}

void tag_collection::dump(tag_deactivator &t_deact) {
    t_deact.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(1250));
}

int tag_collection::size() {
    return t_collection.size();
}