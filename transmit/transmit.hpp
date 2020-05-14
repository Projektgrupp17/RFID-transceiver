#ifndef TRANSMIT_HPP
#define TRANSMIT_HPP

class config;
class tag_data;
namespace Poco {
    namespace JSON {
        class Object;
    }
}

class transmit {
    public:
        void send_tag_to_server(config &conf, tag_data &t_data);
        bool unregister_tag_with_server(config &conf, tag_data &t_data);
    private:
        int calculate_content_length(Poco::JSON::Object &obj);
};

#endif