#include "../simulate/simulate.hpp"
#include "../transmit/transmit.hpp"
#include "../util/util.hpp"
#include "../config/config.hpp"
#include "../data/tag_data.hpp"
#include "../data/tag_collection.hpp"
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>

simulate::simulate(config &con) : conf(con), trans(transmit()), t_col(tag_collection(trans, conf)) {}

void simulate::run() {
    int status = establish_connection();
    tag_data t_data;

    if (status == 0) {
        std::cout << "Connection established successfully.\n"
            << "Selecting protocol.\n";
        status = select_protocol(conf.get_rfid_protocol());

        if (status == 0) {
            int seed = std::stoi(conf.get_sim_seed_no());

            for (int i = 0; i < std::stoi(conf.get_repeat()); i++) {
                scan_for_tags(t_data, false, seed++);

                if (t_data.tag_hex.size()) {
                    t_data.output_tag_summary(t_data);
                    trans.send_tag_to_server(conf, t_data);
                    t_col.connect(t_data);
                }

                if (std::stoi(conf.get_repeat()) - i != 1) {
                    std::cout << "Rescanning in " << conf.get_scan_delay() << "ms.\n";
                    t_data = tag_data();
                    std::this_thread::sleep_for(std::chrono::milliseconds(std::stoi(conf.get_scan_delay())));
                }
            }
        } else {
            std::cout << "RF field activation failed.\n";
        }
    } else {
        std::cout << "Connection to transceiver failed.\n";
    }

}

int simulate::establish_connection(bool status) {
    std::cout << "Establishing connection...\n";
    return status ? 0 : 1;
}

int simulate::select_protocol(std::string protocol, bool status) {
    std::cout << "Activating RF field for passive devices.\nSelected protocol: ISO " << protocol << ".\n";
    return status ? 0 : 1;
}

void simulate::scan_for_tags(tag_data &t_data, bool random, int seed, bool status) {
    std::mt19937_64 rng;
    u_int64_t seed_no;

    if (random) {
        seed_no = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    } else {
        seed_no = (u_int64_t)seed;
    }

    if (status) {
        rng.seed(seed_no);
        char hex[32] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
        char res[32] = {'8', '0', '0', 'D', '0', '0', '0', '0'};
        char extracted_part[32] = "";

        for (int i = 8; i < 28; i++) {
            res[i] = hex[rng() % 16];
        }

        res[28] = '0';
        res[29] = '0';

        t_data.status = 0;

        util::arr_slice(res, extracted_part, 0, 2);
        t_data.status_code = extracted_part;
        util::arr_clear(extracted_part, 2);

        util::arr_slice(res, extracted_part, 2, 4);
        t_data.response_size = extracted_part;
        util::arr_clear(extracted_part, 2);

        util::arr_slice(res, extracted_part, 8, 24);
        t_data.tag_hex = extracted_part;
        t_data.tag_binary = util::hex_to_bin(extracted_part);
        util::arr_clear(extracted_part, 16);

        util::arr_slice(res, extracted_part, 24, 28);
        t_data.crc = extracted_part;
        util::arr_clear(extracted_part, 4);

        t_data.protocol_error_status = "00";
    } else {
        t_data.status = 4;
    }
}