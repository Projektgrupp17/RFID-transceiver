#include "src/reader.hpp"
#include "src/simulator.hpp"
#include "src/util.hpp"
#include "src/config.hpp"
#include <iostream>
#include <vector>
#include <regex>

void display_menu(reader &rdr, simulator &sim, config &conf);
void configure(config &conf, bool first_time = true);
void alert_error(std::string input, std::string reason);
// refactor matcher - match pointer wtf???
void match_params(std::string input, std::regex regex, std::smatch* match, std::string* str);

int main() {
    config conf;
    reader rdr = reader(conf);
    simulator sim = simulator(conf);

    while (conf.run()) {
        display_menu(rdr, sim, conf);
    }
}

void display_menu(reader &rdr, simulator &sim, config &conf) {
    std::cout << "\nChoose option below:\na) configure\nb) run\nc) simulate (note: config applicable to simulations too)\nq) quit\n\nInvalid input or blank line defaults to run.\n";
    char input;
    std::cin >> input;

    if (input == 'a' || input == 'A') {
        configure(conf);
    } else if (input == 'c' || input == 'C') {
        sim.run();
    } else if (input == 'q') {
        conf.quit();
    } else {
        rdr.run();
    }
}

void configure(config &conf, bool first_time) {
    std::regex pattern("^\\s*(::\\s*(protocol|rotate|url|delay|sim(\\s*:\\s*seed|\\s*:\\s*repeat)|quit)||(protocol|rotate|url|delay|sim(\\s*:\\s*seed|\\s*:\\s*repeat)|list)\\s*::)(\\s+([0-9,\\s]+|(http|https)://(www\\.)?([a-zA-Z0-9][a-zA-Z0-9\\-]*\\.?)+((:|/)[\\S]*)?|((http|https)://)?([12]?[0-9]{1,2}\\.){3}([12]?[0-9]{1,2})((:|/)[\\S]*)))*?\\s*$");
    std::smatch match;
    std::smatch p_match;
    std::string input;

    if (first_time) {
        std::string ugly_solution;
        std::cout << "Valid commands utilize '::', prefix to set, suffix to get.\nGlobal commands:\nlist::, ::quit.\n\nSetting specific commands, prepend '::' to command followed by options to set:"
            << "\n::protocol [15693, ...], ::rotate [15693, ...] (comma separated), ::url [http://host:port], ::delay [1000] (ms).\n"
            << "Append '::' to list current setting, i.e.:\ndelay::.\n\nFor futher details do: command --help.\n";
        std::getline(std::cin, ugly_solution);
    }

    std::cin.ignore(0, '\n');
    std::getline(std::cin, input);
    
    if (std::regex_search(input, pattern)) {
        std::regex_search(input, match, pattern);
        std::string cmd;
        std::string options = match[7];
        std::string match_str;
        bool read = false;

        if ((match[2]).str().size()) {
            cmd = match[2];
        } else {
            cmd = match[4];
            read = true;
        }

        if (cmd == "protocol") {
            if (read) {
                std::cout << "Current protocol: " << (conf.is_rotate() ? "disabled (ISO " + conf.get_rfid_protocol() + "). Rotation is enabled.\n" : "ISO " + conf.get_rfid_protocol() + ".\n");
            } else {
                std::regex param("^\\s*(15693|14400)\\s*$");
                match_params(options, param, &p_match, &match_str);

                conf.set_rfid_protocol(match_str);
            }
        } else if (cmd == "rotate") {
            std::vector<std::string> protocols;

            if (read) {
                conf.get_rfid_rotate(protocols);

                std::cout << "Current protocol rotation: " << (conf.is_rotate() ? "" : "disabled (");

                if (protocols.size() > 0) {
                    for (int i = 0; i < (int)protocols.size() - 1; i++) {
                        std::cout << protocols[i] << ", ";
                    }

                    std::cout << protocols[protocols.size() - 1] << (conf.is_rotate() ? ".\n" : ").\n");
                } else {
                    std::cout << "nothing to rotate).\n";
                }
            } else {
                std::string str;
                std::regex param("^\\s*((15693|14400)\\s*,*\\s*)+\\s*$");

                match_params(options, param, &p_match, &match_str);
                util::trim(match_str, str);
                util::split(str, ",", &protocols);

                conf.set_rfid_rotate(protocols);
            }
        } else if (cmd == "url") {
            if (read) {
                std::cout << "Current transmission url: " << conf.get_reg_uri() << "\n";
            } else {
                std::regex param("^\\s*(((http|https)://)?((www\\.)?[a-zA-Z0-9\\.\\-]{1,253}\\.[a-zA-Z0-9\\-]{2,25}|([12]?[0-9]{1,2}\\.){3}([12]?[0-9]{1,2})|localhost)(:[1-9][0-9]{1,4})?(/[a-zA-Z0-9\\-]+)*/?(\\?[a-zA-Z0-9]+=[a-zA-Z0-9]+)*)\\s*$");
                match_params(options, param, &p_match, &match_str);

                conf.set_transmission_uri(match_str);
            }
        } else if (cmd == "delay") {
            if (read) {
                std::cout << "Current scan delay: " << conf.get_scan_delay() << ".\n";
            } else {
                std::regex param("^\\s*([1-9][0-9]+)\\s*$");
                match_params(options, param, &p_match, &match_str);

                conf.set_scan_delay(match_str);
            }
        } else if (cmd == "list") {
            std::cout << "Seed no " << conf.get_sim_seed_no() << " with # number of repeats " << conf.get_repeat() << "\n";
        } else if (cmd == "quit") {
            return;
        } else {
            if (read) {
                std::regex seed(":seed");

                if (std::regex_search(cmd, seed)) {
                    std::cout << "Current seed no.: " << conf.get_sim_seed_no() << ".\n";
                } else {
                    std::cout << "Current # of repeats: " << conf.get_repeat() << ".\n";
                }
            } else {
                std::regex seed(":seed");


                if (std::regex_search(cmd, seed)) {
                    std::regex no("^[0-9]+$");
                    match_params(options, no, &p_match, &match_str);
                    
                    conf.set_sim_seed_no(match_str);
                } else {
                    std::regex no("^[0-9]+$");
                    match_params(options, no, &p_match, &match_str);

                    conf.set_repeat(match_str);
                }
            }
        }
    } else {
        alert_error(input, "command not found");
    }

    configure(conf, false);
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