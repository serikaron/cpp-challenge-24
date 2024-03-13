//
// Created by serika on 2024/3/13.
//

#include <catch2/catch_test_macros.hpp>
#include "Broker.h"
#include <vector>
#include <algorithm>

TEST_CASE("Receive a request", "[Broker]") {
    std::vector lines{{
                              "PATCH /profile/aambertin HTTP/1.1",
                              "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)",
                              "Host: dev-challenges.kerno.io",
                              "Accept-Language: en-us",
                              "X-Trace-ID: CAcICAYI",
                              " ",
                      }};

    Broker broker;

    std::vector<Message> messages;
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(messages),
                   [&broker](std::string &&line) {
                       return broker.handleInput(std::move(line));
                   });

    std::vector<Message> noneMessages;
    std::ranges::for_each(messages.begin(), messages.end() - 1, [](const auto &message) {
        REQUIRE(message.messageType == MessageType::none);
    });

    const auto &requestMessage = messages.back();
    REQUIRE(requestMessage.messageType == MessageType::request);
    REQUIRE(requestMessage.path == "/profile/aambertin");
    REQUIRE(requestMessage.id == "CAcICAYI");
}

TEST_CASE("Receive a response", "[Broker]") {
    std::vector lines{{
                              "HTTP/1.1 501 NOT IMPLEMENTED",
                              "Date: Wed Mar 13 2024 09:19:27 GMT+0000",
                              "Server: nginx",
                              "X-Trace-ID: CQMIAwQJ",
                              "Connection: Closed",
                              " ",
                      }};

    Broker broker;

    std::vector<Message> messages;
    std::transform(lines.begin(), lines.end(),
                   std::back_inserter(messages),
                   [&broker](std::string &&line) {
                       return broker.handleInput(std::move(line));
                   });

    std::vector<Message> noneMessages;
    std::ranges::for_each(messages.begin(), messages.end() - 1, [](const auto &message) {
        REQUIRE(message.messageType == MessageType::none);
    });

    const auto &responseMessage = messages.back();
    REQUIRE(responseMessage.messageType == MessageType::response);
    REQUIRE(responseMessage.code == "501");
    REQUIRE(responseMessage.id == "CQMIAwQJ");
}