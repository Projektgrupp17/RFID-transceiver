#include "config.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <regex>

config::config() {
    status = true;
    id.location_id = "123";
    id.rfid_reader_id = "456";
    transmit.hostname = "127.0.0.1";
    transmit.port = "1234";
    reader.protocol = "15693";
    reader.rotate_protocol = false;
    reader.scan_delay = "1000";
    reader.repeat = "1";
    conf.select_protocol = "01";
    conf.select_parameters = "0D";
    conf.send_data = "260100";
    sim.seed_no = "1";
}

bool config::run() {
    return status;
}

void config::quit() {
    status = false;
}

std::string config::get_location_id() {
    return id.location_id;
}

std::string config::get_rfid_reader_id() {
    return id.rfid_reader_id;
}

std::string config::get_rfid_protocol() {
    return reader.protocol;
}

void config::get_rfid_rotate(std::vector<std::string> &protocols) {
    for (int i = 0; i < (int)reader.rotate.size(); i++) {
        protocols.push_back(reader.rotate[i]);
    }
}

bool config::is_rotate() {
    return reader.rotate_protocol;
}

std::string config::get_rfid_select_protocol_confbits() {
    return conf.select_protocol;
}

std::string config::get_rfid_select_parameters_confbits() {
    return conf.select_parameters;
}

std::string config::get_rfid_send_data_confbits() {
    return conf.send_data;
}

std::string config::get_transmission_uri() {
    return transmit.original_url;
}

std::string config::get_transmission_hostname() {
    return transmit.hostname;
}

std::string config::get_transmission_path() {
    return transmit.path.size() ? transmit.path : "/";
}

std::string config::get_transmission_port() {
    return transmit.port.size() ? transmit.port : "80";
}

std::string config::get_sim_seed_no() {
    return sim.seed_no;
}

std::string config::get_scan_delay() {
    return reader.scan_delay;
}

std::string config::get_repeat() {
    return reader.repeat;
}

void config::set_location_id(std::string loc_id) {
    id.location_id = loc_id;
}

void config::set_rfid_reader_id(std::string rfid_id) {
    id.rfid_reader_id = rfid_id;
}

void config::set_rfid_protocol(std::string protocol) {
    reader.rotate_protocol = false;

    if (protocol == "15693") {
        std::cout << "Setting protocol: " << protocol << "\n";
        reader.protocol = "15693";
        conf.select_protocol = "01";
        conf.select_parameters = "0D";
        conf.send_data = "260100";
    } else if (protocol == "14443A") {
        reader.protocol = "14443A";
        conf.select_protocol = "02";
    } else if (protocol == "14443B") {
        reader.protocol = "14443B";
        conf.select_protocol = "03";
    } else if (protocol == "18092") {
        reader.protocol = "18092";
        conf.select_protocol = "04";
    }
}

void config::set_rfid_rotate(std::vector<std::string> protocols) {
    if (protocols.size() == 1) {
        std::cout << "One protocol specified. Nothing to rotate. Setting default protocol.\n";
        set_rfid_protocol(protocols[0]);
        return;
    }

    reader.rotate.clear();
    reader.rotate_protocol = true;

    for (int i = 0; i < (int)protocols.size(); i++) {
        bool exists = false;

        for (int j = 0; j < (int)reader.rotate.size(); j++) {
            if (reader.rotate[j] == protocols[i]) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            reader.rotate.push_back(protocols[i]);
        }
    }

    std::cout << "Rotation mode enabled. Protocols to rotate: ";

    for (int i = 0; i < (int)(reader.rotate.size() - 1); i++) {
        std::cout << reader.rotate[i] << ", ";
    }

    std::cout << reader.rotate[reader.rotate.size() - 1] << ".\n";
}

void config::set_transmission_uri(std::string url) {
    transmit.original_url = url;
    std::regex rest("^((http|https)://)?(www.)?(([A-Za-z0-9\\-]+\\.)+([A-Za-z]+))(:([0-9]{1,5}))?((/[a-zA-Z0-9\\-]*)*/?)$");
    std::smatch match;

    std::regex_search(url, match, rest);
    transmit.protocol = match[2];
    transmit.hostname = match[4];
    transmit.port = match[8];
    transmit.path = match[9];
}

void config::set_scan_delay(std::string delay) {
    std::cout << "Setting scan delay: " << delay << " ms.\n";
    reader.scan_delay = delay;
}

void config::set_repeat(std::string repeat) {
    reader.repeat = repeat;
}

void config::set_sim_seed_no(std::string seed_no) {
    sim.seed_no = seed_no;
}