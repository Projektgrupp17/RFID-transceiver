#include <iostream>
#include <string>
#include <vector>

class Config {
    private:
        bool status;
        std::string rfid_protocol;
        std::vector<std::string> rfid_rotate;
        std::string transmission_url;
        bool rotate_protocols;
        int scan_delay;

    public:
        Config() {
            status = true;
            rfid_protocol = "15693";
            rotate_protocols = false;
            scan_delay = 1000;
        }

        bool run() {
            return status;
        }

        void quit() {
            status = false;
        }

        std::string get_rfid_protocol() {
            return rfid_protocol;
        }

        void get_rfid_rotate(std::vector<std::string> &protocols) {
            for (int i = 0; i < (int)rfid_rotate.size(); i++) {
                protocols.push_back(rfid_rotate[i]);
            }
        }

        std::string get_transmission_url() {
            return transmission_url;
        }

        bool is_rotate() {
            return rotate_protocols;
        }

        int get_scan_delay() {
            return scan_delay;
        }

        void set_rfid_protocol(std::string protocol) {
            rotate_protocols = false;

            if (protocol == "15693") {
                std::cout << "Setting protocol: " << protocol << "\n";
                rfid_protocol = "15693";
            } else if (protocol == "14400") {
                std::cout << "Cat eats dog. Over and out.\n";
            } else {
                std::cout << "Invalid protocol type, defaulting to ISO/IEC 15693.\n";
                rfid_protocol = "15693";
            }
        }

        void set_rfid_rotate(std::vector<std::string> protocols) {
            if (protocols.size() == 1) {
                std::cout << "One protocol specified. Nothing to rotate. Setting default protocol.\n";
                set_rfid_protocol(protocols[0]);
                return;
            }

            rfid_rotate.clear();
            rotate_protocols = true;

            for (int i = 0; i < (int)protocols.size(); i++) {
                bool exists = false;

                for (int j = 0; j < (int)rfid_rotate.size(); j++) {
                    if (rfid_rotate[j] == protocols[i]) {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    rfid_rotate.push_back(protocols[i]);
                }
            }

            std::cout << "Rotation mode enabled. Protocols to rotate: ";

            for (int i = 0; i < (int)(rfid_rotate.size() - 1); i++) {
                std::cout << rfid_rotate[i] << ", ";
            }

            std::cout << rfid_rotate[rfid_rotate.size() - 1] << ".\n";
        }

        void set_transmission_url(std::string url) {
            std::cout << "Setting transmission url: " << url << ".\n";
            transmission_url = url;
        }

        void set_scan_delay(int delay) {
            std::cout << "Setting scan delay: " << delay << " ms.\n";
            scan_delay = delay;
        }
};