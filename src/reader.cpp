#include "reader.hpp"
#include "config.hpp"
#include "tag_data.hpp"
#include "util.hpp"
#include "transmitter.hpp"
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include "../include/libcr95hf.h"

reader::reader(config &con) : conf(con), trans(transmitter()), t_col(tag_collection(conf, trans)), w_man(work_manager(t_col)), t_deact(tag_deactivator(conf, trans, w_man, this)) {
    deactivator_ready = false;
};

void reader::run() {
    int status = establish_connection();
    tag_data t_data;

    if (status == 0) {
        std::cout << "Connection established successfully.\n"
            << "Selecting protocol.\n";
        status = select_protocol();

        if (status == 0) {
            t_deact.set_master(false);

            for (int i = 0; i < std::stoi(conf.get_repeat()); i++) {
                if (deactivator_ready) {
                    w_man.handle_msg(t_deact);
                    toggle_deactivator_ready();
                }

                scan_for_tags(t_data);

                if (t_data.tag_hex.size()) {
                    t_data.output_tag_summary(t_data);
                    trans.register_tag_with_server(conf, t_data);
                }

                if (std::stoi(conf.get_repeat()) - i != 1) {
                    std::cout << "Rescanning in " << conf.get_scan_delay() << "ms.\n";
                    t_data = tag_data();
                    std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(conf.get_scan_delay())));
                }

                if (deactivator_ready) {
                    w_man.handle_msg(t_deact);
                    toggle_deactivator_ready();
                }
            }

            t_deact.set_master(true);
        } else {
            std::cout << "RF field activation failed.\n";
        }

    } else {
        std::cout << "Connection to transceiver failed.\n";
    }
}

int reader::establish_connection() {
    std::cout << "Establishing connection...\n";
    return CR95HFlib_USBConnect();
}

int reader::select_protocol() {
    char req[32] = "", res[32] = "";
    strcpy(req, (conf.get_rfid_select_protocol_confbits() + conf.get_rfid_select_parameters_confbits()).c_str());
    std::cout << "Activating RF field for passive devices.\nSelected protocol: ISO " << conf.get_rfid_protocol() << ".\n";
    return CR95HFlib_Select(req, res);
}

void reader::scan_for_tags(tag_data &t_data) {
    char req[32] = "", res[32] = "", extracted_part[32] = "";
    strcpy(req, conf.get_rfid_send_data_confbits().c_str());
    t_data.status = CR95HFlib_SendReceive(req, res);

    util::arr_slice(res, extracted_part, 0, 2);
    t_data.status_code = extracted_part;
    util::arr_clear(extracted_part, 2);

    util::arr_slice(res, extracted_part, 2, 4);
    t_data.response_size = extracted_part;
    util::arr_clear(extracted_part, 2);

    util::arr_slice(res, extracted_part, 4, 24);
    t_data.tag_hex = extracted_part;
    t_data.tag_binary = util::hex_to_bin(extracted_part);
    util::arr_clear(extracted_part, 20);

    util::arr_slice(res, extracted_part, 24, 28);
    t_data.crc = extracted_part;
    util::arr_clear(extracted_part, 4);

    util::arr_slice(res, extracted_part, 28, 29);
    t_data.protocol_error_status = extracted_part;
}

void reader::toggle_deactivator_ready() {
    deactivator_ready = !deactivator_ready;
}