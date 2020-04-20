#include "reader/reader.hpp"
#include "simulate/simulate.hpp"
#include "util/util.hpp"
#include "data/TagData.hpp"
#include "config/Config.hpp"
#include <iostream>
#include <vector>
#include <regex>

void display_menu(Config &config);
void configure(Config &config, bool first_time = true);
void run(Config &config);
void simulate(Config &config);
void alert_error(std::string input, std::string reason);
// refactor matcher - match pointer wtf???
void match_params(std::string input, std::regex regex, std::smatch* match, std::string* str);
void summary(TagData &tag_data);

int main() {
    Config config;

    while (config.run()) {
        display_menu(config);
    }
}

void display_menu(Config &config) {
    std::cout << "\nChoose option below:\na) configure\nb) run\nc) simulate (note: config applicable to simulations too)\nq) quit\n\nInvalid input or blank line defaults to run.\n";
    char input;
    std::cin >> input;

    if (input == 'a' || input == 'A') {
        configure(config);
    } else if (input == 'c' || input == 'C') {
        simulate(config);
    } else if (input == 'q') {
        config.quit();
    } else {
        run(config);
    }
}

void configure(Config &config, bool first_time) {
    std::regex pattern("^\\s*(::\\s*(protocol|rotate|url|delay|quit)||(protocol|rotate|url|delay|list)\\s*::)(\\s+([\\w,\\s]+|(http|https)://(www\\.)?([a-zA-Z0-9][a-zA-Z0-9\\-]*\\.?)+((:|/)[\\S]*)?)*)?\\s*$");
    std::smatch match;
    std::smatch p_match;
    std::string input;

    if (first_time) {
        std::cout << "Valid commands utilize '::', prefix to set, suffix to get.\nGlobal commands:\nlist::, ::quit.\n\nSetting specific commands, prepend '::' to command followed by options to set:"
            << "\n::protocol [15693, ...], ::rotate [15693, ...] (comma separated), ::url [http://host:port], ::delay [1000] (ms).\n"
            << "Append '::' to list current setting, i.e.:\ndelay::.\n\nFor futher details do: command --help.\n";
    }

    std::cin.ignore(0, '\n');
    std::getline(std::cin, input);

    if (std::regex_search(input, pattern)) {
        std::regex_search(input, match, pattern);
        std::string cmd;
        std::string options = match[5];
        std::string match_str;
        bool read = false;

        if ((match[2]).str().size()) {
            cmd = match[2];
        } else {
            cmd = match[3];
            read = true;
        }

        if (cmd == "protocol") {
            if (read) {
                std::cout << "Current protocol: " << (config.is_rotate() ? "disabled (ISO " + config.get_rfid_protocol() + "). Rotation is enabled.\n" : "ISO " + config.get_rfid_protocol() + ".\n");
            } else {
                std::regex param("^\\s*(15693|14400)\\s*$");
                match_params(options, param, &p_match, &match_str);

                config.set_rfid_protocol(match_str);
            }
        } else if (cmd == "rotate") {
            std::vector<std::string> protocols;

            if (read) {
                config.get_rfid_rotate(protocols);

                std::cout << "Current protocol rotation: " << (config.is_rotate() ? "" : "disabled (");

                if (protocols.size() > 0) {
                    for (int i = 0; i < (int)protocols.size() - 1; i++) {
                        std::cout << protocols[i] << ", ";
                    }

                    std::cout << protocols[protocols.size() - 1] << (config.is_rotate() ? ".\n" : ").\n");
                } else {
                    std::cout << "nothing to rotate).\n";
                }
            } else {
                std::string str;
                std::regex param("^\\s*((15693|14400)\\s*,*\\s*)+\\s*$");

                match_params(options, param, &p_match, &match_str);
                trim(match_str, &str);
                split(str, ",", &protocols);

                config.set_rfid_rotate(protocols);
            }
        } else if (cmd == "url") {
            if (read) {
                std::cout << "Current transmission url: " << config.get_transmission_url() << "\n";
            } else {
                std::regex param("^\\s*((http|https)://(www\\.)?[a-zA-Z0-9\\.\\-]{1,253}\\.[a-zA-Z0-9\\-]{2,25}(:[1-9][0-9]{1,4})?(/[a-zA-Z0-9\\-]+)*(\\?[a-zA-Z0-9]+=[a-zA-Z0-9]+)*)\\s*$");
                match_params(options, param, &p_match, &match_str);

                config.set_transmission_url(match_str);
            }
        } else if (cmd == "delay") {
            if (read) {
                std::cout << "Current scan delay: " << config.get_scan_delay() << ".\n";
            } else {
                std::regex param("^\\s*([1-9][0-9]+)\\s*$");
                match_params(options, param, &p_match, &match_str);

                config.set_scan_delay(std::stoi(match_str));
                std::cout << "setting delay " << p_match[0] << "\n";
            }
        } else if (cmd == "list") {

        } else if (cmd == "quit") {
            return;
        }
    } else {
        alert_error(input, "command not found");
    }

    configure(config, false);
}

void run(Config &config) {
    int status = establish_connection();
    TagData tag_data;

    if (status == 0) {
        std::cout << "Connection established successfully.\n"
            << "Selecting protocol.\n";
        status = select_protocol(config.get_rfid_protocol());
        std::cout << "status " << status << "\n";
        if (status == 0) {
            scan_for_tags(tag_data);
        } else {
            std::cout << "RF field activation failed.\n";
        }

        std::cout << "Rescanning in " << config.get_scan_delay() << "ms.\n";
    } else {
        std::cout << "Connection failed, try again.\n";
    }
}

void simulate(Config &config) {
    int status = simulate_establish_connection();
    TagData tag_data;

    if (status == 0) {
        std::cout << "Connection established successfully.\n"
            << "Selecting protocol.\n";
        status = simulate_select_protocol(config.get_rfid_protocol());

        if (status == 0) {
            simulate_scan_for_tags(tag_data);

            if (tag_data.tag_hex.size()) {
                summary(tag_data);
            }
        } else {
            std::cout << "RF field activation failed.\n";
        }

        std::cout << "Rescanning in " << config.get_scan_delay() << "ms.\n";
    } else {
        std::cout << "Connection failed, try again.\n";
    }
}

void alert_error(std::string input, std::string reason) {
    std::cout << input << ": " << reason << "\n\n";
}

void match_params(std::string input, std::regex regex, std::smatch* match, std::string* match_str) {
    if (std::regex_search(input, regex)) {
        std::regex_search(input, *match, regex);
        *match_str = (*match)[0];
    } else {
        alert_error(input, "invalid parameters");
    }
}

void summary(TagData &tag_data) {
    std::cout << "Tag uid: 0x" << tag_data.tag_hex
        << "\nTag uid binary: " << tag_data.tag_binary << "\nCRC: " << tag_data.crc << "\n";
}