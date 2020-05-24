#include "transmitter.hpp"
#include "tag_data.hpp"
#include "config.hpp"
#include <iostream>
#include "HTTPClientSession.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "StreamCopier.h"
#include "Exception.h"
#include "NetException.h"
#include "Object.h"

void transmitter::register_tag_with_server(config &conf, tag_data &t_data) {
    std::cout << "Transmitting payload to server.\n";
    int port = std::stoi(conf.get_reg_port());
    Poco::Net::HTTPClientSession session(conf.get_reg_hostname(), port);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, conf.get_reg_path(), Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::JSON::Object obj;
    Poco::Net::HTTPResponse response;
    request.setKeepAlive(true);
    request.setContentType("application/json");
    obj.set("loc", conf.get_location_id());
    obj.set("tag", t_data.tag_hex);
    request.setContentLength(calculate_content_length(obj));

    try {
        std::ostream &ostr = session.sendRequest(request);
        obj.stringify(ostr);
        std::istream &res = session.receiveResponse(response);
        Poco::StreamCopier::copyStream(res, std::cout);
        std::cout << "\nStatus: " << response.getStatus() << " | Reason: " << response.getReason() << "\n";
    } catch (Poco::Exception e) {
        std::cerr << "ERROR: " << e.message() << "\n";
    }
    
}

bool transmitter::unregister_tag_with_server(config &conf, tag_data &t_data) {
    std::cout << "Unregistering tag with server.\n";
    int port = std::stoi(conf.get_reg_port());
    Poco::Net::HTTPClientSession session(conf.get_unreg_hostname(), port);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, conf.get_unreg_path(), Poco::Net::HTTPMessage::HTTP_1_1);
    Poco::JSON::Object obj;
    Poco::Net::HTTPResponse response;
    bool status = 0;
    request.setKeepAlive(true);
    request.setContentType("application/json");
    obj.set("loc", conf.get_location_id());
    obj.set("tag", t_data.tag_hex);
    request.setContentLength(calculate_content_length(obj));

    try {
        std::ostream &ostr = session.sendRequest(request);
        obj.stringify(ostr);
        std::istream &res = session.receiveResponse(response);
        Poco::StreamCopier::copyStream(res, std::cout);
        std::cout << "\nStatus: " << response.getStatus() << " | Reason: " << response.getReason() << "\n";
    } catch (Poco::Exception e) {
        std::cerr << "ERROR: " << e.message() << "\n";
        status = 1;
    }

    return status ? false : true;
}

int transmitter::calculate_content_length(Poco::JSON::Object &obj) {
    int len = 0;
    
    for (auto i = obj.begin(); i != obj.end(); i++) {
        len += i->first.length() + i->second.toString().length() + 3;

        if (i->second.isString()) {
            len += 2;
        }
    }

    len += obj.size() + 1;

    return len;
}