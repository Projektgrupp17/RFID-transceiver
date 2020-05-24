#include "work_manager.hpp"
#include "tag_collection.hpp"
#include "tag.hpp"
#include "../tag_deactivator/tag_deactivator.hpp"

work_manager::work_manager(tag_collection &t_co) : t_col(t_co) {};

void work_manager::store_msg(std::pair<std::string, bool> mesg) {
    msg.push_back(mesg);
}

void work_manager::handle_msg(tag_deactivator &t_deact) {
    auto iter = msg.begin();

    for (;iter != msg.end(); iter++, msg.pop_front()) {
        if (iter->first == "REQ") {
            if (t_col.size()) {
                handle_work_delegation(t_deact);
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
}

void work_manager::handle_work_delegation(tag_deactivator &t_deact) {
/*    std::unique_lock<std::mutex> lock_t_collection(mutex_t_collection, std::defer_lock);
    while (!lock_t_collection.try_lock()) {} */

    while (t_col.time_to_live() < 1250) {
        tag_data temp;
        tag t = tag(temp);
        t_col.disconnect(t);
        t_deact.enqueue(t);
    }

    if (t_col.size()) {
        t_deact.sleep(t_col.time_to_live());
    } else {
        t_deact.graceful_shutdown();
    }

    //    lock_t_collection.unlock();
}