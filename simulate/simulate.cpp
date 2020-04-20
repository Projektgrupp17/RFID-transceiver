#include "../util/util.hpp"
#include "../data/TagData.hpp"
#include <iostream>
#include <string>
#include <random>
#include <chrono>

int simulate_establish_connection(bool status = true) {
    std::cout << "Establishing connection...\n";
    return status ? 0 : 1;
}

int simulate_select_protocol(std::string protocol, bool status = true) {
    std::cout << "Activating RF field for passive devices.\nSelected protocol: ISO " << protocol << ".\n";
    return status ? 0 : 1;
}

void simulate_scan_for_tags(TagData &tag_data, bool random = false, int seed = 1, bool status = true) {
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
        char res[32] = {'8', '0', '0', 'D'};
        char extracted_part[32] = "";

        for (int i = 4; i < 28; i++) {
            res[i] = hex[rng() % 16];
        }

        res[28] = '0';
        res[29] = '0';

        tag_data.status = status ? 0 : 1;

        arr_slice(res, extracted_part, 0, 2);
        tag_data.status_code = extracted_part;
        arr_clear(extracted_part, 2);

        arr_slice(res, extracted_part, 2, 4);
        tag_data.response_size = extracted_part;
        arr_clear(extracted_part, 2);

        arr_slice(res, extracted_part, 4, 24);
        tag_data.tag_hex = extracted_part;
        tag_data.tag_binary = hex_to_bin(extracted_part);
        arr_clear(extracted_part, 20);

        arr_slice(res, extracted_part, 24, 28);
        tag_data.crc = extracted_part;
        arr_clear(extracted_part, 4);

        tag_data.protocol_error_status = "00";
    } else {
        tag_data.status = 4;
    }
}