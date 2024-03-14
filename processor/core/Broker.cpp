//
// Created by serika on 2024/3/13.
//

#include "Broker.h"
#include <regex>
#include <iostream>
#include <ranges>

Message Broker::handleInput(std::string &&line) {
    auto trim = [](unsigned char c) { return !std::isspace(c); };
    line.erase(
            std::ranges::find_if(line | std::views::reverse, trim).base(),
            line.end()
    );

    if (!line.empty()) {
        block.push_back(std::move(line));
        return Message::none();
    }

    Message out =
            isRequestBlock() ? Message::request(std::move(getID()), std::move(getPath()))
                             : isResponseBlock() ? Message::response(std::move(getID()), std::move(getCode()))
                                                 : Message::none();

    block.clear();

    return out;
}

bool Broker::isRequestBlock() const {
    return !getPath().empty();
}

std::string Broker::getPath() const {
    if (block.empty()) {
        return "";
    }

    std::smatch matches;
    const std::regex r(R"((\w+) (\S+) HTTP/\d\.\d)");
    if (std::regex_match(block.front(), matches, r)
        && matches.size() == 3) {
        return matches[2];
    }

    return "";
}


std::string Broker::getID() const {
    const std::regex r(R"(X-Trace-ID: (\S+))");

    for (const std::string &line: block) {
        std::smatch matches;
        if (std::regex_match(line, matches, r)
            && matches.size() == 2) {
            return matches[1];
        }
    }

    return "";
}

std::string Broker::getCode() const {
    if (block.empty()) {
        return "";
    }

    std::smatch matches;
    const std::regex r(R"(HTTP/\d\.\d (\d+) (.+))");
    if (std::regex_match(block.front(), matches, r)
        && matches.size() == 3) {
        return matches[1];
    }

    return "";
}

bool Broker::isResponseBlock() const {
    return !getCode().empty();
}
