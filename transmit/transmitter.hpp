#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

class config;
class tag_data;
namespace Poco {
    namespace JSON {
        class Object;
    }
}

class transmitter {
    public:
        void register_tag_with_server(config &conf, tag_data &t_data);
        bool unregister_tag_with_server(config &conf, tag_data &t_data);
    private:
        int calculate_content_length(Poco::JSON::Object &obj);
};

#endif