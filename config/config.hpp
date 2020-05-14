#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "../data/config_id.hpp"
#include "../data/config_url.hpp"
#include "../data/config_reader.hpp"
#include "../data/config_bitconf.hpp"
#include "../data/config_sim.hpp"
#include <string>
#include <vector>

class config {
    private:
        config_id id;
        config_url transmit;
        config_reader reader;
        config_bitconf conf;
        config_sim sim;
        bool status;

    public:
        config();
        bool run();
        void quit();
        bool is_rotate();
        std::string get_location_id();
        std::string get_rfid_reader_id();
        std::string get_rfid_protocol();
        void get_rfid_rotate(std::vector<std::string> &protocols);
        std::string get_rfid_select_protocol_confbits();
        std::string get_rfid_select_parameters_confbits();
        std::string get_rfid_send_data_confbits();
        std::string get_reg_uri();
        std::string get_reg_hostname();
        std::string get_reg_path();
        std::string get_reg_port();
        std::string get_unreg_uri();
        std::string get_unreg_hostname();
        std::string get_unreg_path();
        std::string get_unreg_port();
        std::string get_scan_delay();
        std::string get_time_to_live();
        std::string get_repeat();
        std::string get_sim_seed_no();
        void set_location_id(std::string loc_id);
        void set_rfid_reader_id(std::string rfid_id);
        void set_rfid_protocol(std::string protocol);
        void set_rfid_rotate(std::vector<std::string> protocols);
        void set_transmission_uri(std::string url);
        void set_scan_delay(std::string delay);
        void set_repeat(std::string repeat);
        void set_sim_seed_no(std::string);
};

#endif