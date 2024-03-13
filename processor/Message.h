//
// Created by serika on 2024/3/13.
//

#ifndef PROCESSOR_MESSAGE_H
#define PROCESSOR_MESSAGE_H

#include <string>

enum MessageType {
    none, request, response
};

struct Message {
    std::string id;
    MessageType messageType;
    std::string path;
    std::string code;

    static Message none() {
        return Message{"", MessageType::none, "", ""};
    }

    static Message request(std::string && id, std::string &&path) {
        return Message{std::move(id), MessageType::request, std::move(path), ""};
    }

    static Message response(std::string &&id, std::string &&code) {
        return Message{std::move(id), MessageType::response, "", std::move(code)};
    }
};

#endif //PROCESSOR_MESSAGE_H
