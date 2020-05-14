#include "tag_deactivator.hpp"
#include "tag_collection.hpp"
#include "../transmit/transmit.hpp"
#include "../config/config.hpp"
#include "../data/tag_data.hpp"
#include <thread>

tag_deactivator::tag_deactivator(transmit &trns, config &con, tag_collection *t_co) : data(tag_data()), conf(con), trs(trns), t_col(t_co) {
    interval = 1000;
    sleep_duration = 0;
    postponed = false;
    active = false;
    asleep = false;
    data.tag_hex = "ABBA1234";
}

void tag_deactivator::run() {
    if (!active) {
        active = true;
        std::function<void()> callb = [this]{ deactivate(this, this->t_col, this->trs, this->conf); };
        thr = std::thread(callb);
        thr.detach();
    }
}

void tag_deactivator::close() {
    if (active) {
        active = false;
    }
}

void tag_deactivator::graceful_shutdown() {
    std::function<void()> callb = [this]{ shutdown(this); };
}

void tag_deactivator::sleep(int ms) {
    int min_1ms = ms == 0 ? ms + 1 : ms;
    sleep_duration = asleep ? min_1ms : (sleep_duration + min_1ms);
}

void tag_deactivator::postpone() {
    postponed = true;
}

void tag_deactivator::set_interval(int ms) {
    interval = ms;
}

void tag_deactivator::enqueue(tag &tg) {
    queue.push_back(tg);
}

bool tag_deactivator::is_active() {
    return active;
}

bool tag_deactivator::is_asleep() {
    return asleep;
}

void tag_deactivator::deactivate(tag_deactivator *t_deact, tag_collection *t_col, transmit trs, config conf) {

    while (t_deact->active) {
        if (t_deact->queue.size()) {
            auto iter = t_deact->queue.begin();
            tag &t = *iter;
            tag_data data = t.get_data();
            t_deact->queue.pop_front();

            bool status = trs.unregister_tag_with_server(conf, data);

            if (!t_deact->active) {
                break;
            }

            std::pair<std::string, bool> pair = std::make_pair(data.tag_hex, status);
            t_col->store_msg(pair);
}       else if (!(t_deact->sleep_duration)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(t_deact->interval));
        } else {
            int slp = t_deact->sleep_duration;
            t_deact->sleep_duration = 0;
            t_deact->asleep = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(slp));
            t_deact->asleep = false;

            if (!t_deact->active) {
                break;
            }

            if (!t_deact->postponed) {
                std::pair<std::string, bool> pair = std::make_pair("REQ", true);
                t_col->store_msg(pair);
            }
            
            t_deact->postponed = false;
        }
    }
}

void tag_deactivator::shutdown(tag_deactivator *t_deact) {
    while (t_deact->queue.size()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    t_deact->active = false;
}