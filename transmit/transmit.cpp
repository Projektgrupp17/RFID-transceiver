#include "transmit.hpp"
#include "../data/tag_data.hpp"
#include "../config/config.hpp"
#include <iostream>
#include "HTTPClientSession.h"
#include "HTTPRequest.h"
#include "HTTPResponse.h"
#include "StreamCopier.h"
#include "Exception.h"
#include "NetException.h"
#include "Object.h"

void transmit::send_tag_to_server(config &conf, tag_data &t_data) {
    std::cout << "Transmitting payload to server.\n";
    std::cout << conf.get_transmission_port() << " " << conf.get_transmission_hostname() << " " << conf.get_transmission_path() <<"\n";
    int port = std::stoi(conf.get_transmission_port());
    Poco::Net::HTTPClientSession session(conf.get_transmission_hostname(), port);
    Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, conf.get_transmission_path(), Poco::Net::HTTPMessage::HTTP_1_1);
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

int transmit::calculate_content_length(Poco::JSON::Object &obj) {
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