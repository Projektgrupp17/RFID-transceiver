#include "../data/tag_collection.hpp"
#include "../data/tag_data.hpp"
#include "../data/tag.hpp"
#include "../transmit/transmit.hpp"
#include "../config/config.hpp"
#include <thread>

tag_collection::tag_collection(transmit &trs, config &con) : trans(trs), conf(con), t_deact(tag_deactivator(trans, conf, this)) {

};

tag_collection::tag_collection(tag_data &data, transmit &trs, config &con) : trans(trs), conf(con), t_deact(tag_deactivator(trans, conf, this)) {
    tag t = tag(data);

    connect(data);
}

void tag_collection::connect(tag_data &data) {
    auto index = index_of(data);
    tag t = tag(data);
    long long int time_to_live = (std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1)) + std::stoi(conf.get_time_to_live());
    int sleep = std::stoi(conf.get_time_to_live());
//    std::future<std::pair<std::string, bool>> ftr = std::async(std::launch::async, deactivate, data, ttl, *this, trans, conf);
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

void tag_collection::store_msg(std::pair<std::string, bool> mesg) {
    msg.push_back(mesg);
    handle_msg();
}

void tag_collection::handle_msg() {
    auto iter = msg.begin();

    for (;iter != msg.end(); iter++, msg.pop_front()) {
        if (iter->first == "REQ") {
            if (t_collection.size()) {
            /*    std::unique_lock<std::mutex> lock_t_collection(mutex_t_collection, std::defer_lock);
                while (!lock_t_collection.try_lock()) {} */
                auto iter = t_collection.begin();
                std::pair<tag, long long int> pair = *iter;
                t_collection.pop_front();
                t_deact.enqueue(pair.first);
            //    lock_t_collection.unlock();
            } else {
                t_deact.graceful_shutdown();
            }
        } else {
            if (iter->second) {
                std::cout << "Tag uid " << iter->first << " has left the premises.\n";
            } else {
                std::cout << "Deactivation of tag " << iter->first << " failed.";
            }
        }
    }

    handle_work_delegation();
}

void tag_collection::handle_work_delegation() {
    long long int time_now = std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    auto iter = t_collection.begin();
/*    std::unique_lock<std::mutex> lock_t_collection(mutex_t_collection, std::defer_lock);
    while (!lock_t_collection.try_lock()) {} */
    for (; iter != t_collection.end() && (iter->second - time_now) < 1250; iter++, t_collection.pop_front()) {
        std::pair<tag, long long int> pair = *iter;
        t_deact.enqueue(pair.first);
    }
//    lock_t_collection.unlock();

    if (t_collection.size()) {
        time_now = std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
        iter = t_collection.begin();
        int sleep = iter->second - time_now;
        t_deact.sleep(sleep);
    } else {
        t_deact.graceful_shutdown();
    }
}

void tag_collection::dump() {
    t_deact.close();
    std::this_thread::sleep_for(std::chrono::milliseconds(1250));
}

int tag_collection::size() {
    return t_collection.size();
}