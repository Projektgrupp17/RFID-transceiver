#include "../util/util.hpp"
#include "../data/TagData.hpp"
#include <iostream>
#include <cstring>
#include "../lib/libcr95hf.h"

int establish_connection() {
    std::cout << "Establishing connection...\n";
    return CR95HFlib_USBConnect();
}

int select_protocol(std::string protocol) {
    char req[32] = "", res[32] = "";
    strcpy(req, "010D");
    std::cout << "Activating RF field for passive devices.\nSelected protocol: ISO " << protocol << ".\n";
    int status = CR95HFlib_Select(req, res);
    return status;
}

void scan_for_tags(TagData tag_data) {
    char req[32] = "", res[32] = "", extracted_part[32] = "";
    strcpy(req, "260100");
    tag_data.status = CR95HFlib_SendReceive(req, res);

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

    arr_slice(res, extracted_part, 28, 30);
    tag_data.protocol_error_status = extracted_part;
}