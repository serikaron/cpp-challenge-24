//
// Created by serika on 2024/3/13.
//

#include "catch2/catch_test_macros.hpp"
#include "../core/Broker.h"
#include "../core/Mapping.h"
#include <vector>
#include <algorithm>
#include <sstream>
#include <thread>
#include <future>

TEST_CASE("Broker tests", "[Broker]") {
    std::stringstream ss;
    std::shared_ptr<Mapping> mapping = std::make_shared<Mapping>();
    Broker broker{ss, mapping};

    std::mutex mutex;
    auto brokerRun = [&broker, &mutex, &ss](const std::string &messageBlock) {
        ss.clear();
        ss << messageBlock;
        std::async([&broker, &mutex] {
            std::scoped_lock<std::mutex> lock(mutex);
            broker.run();
        })
                .wait();
    };

    // receive first request
    {
        brokerRun("PUT /feed/smadigan HTTP/1.1\n"
                  "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
                  "Host: dev-challenges.kerno.io\n"
                  "Accept-Language: en-us\n"
                  "X-Trace-ID: BgYHAgYB\n"
                  " ");

        auto [pending, completed] = mapping->getStat();
        REQUIRE(pending == 1);
        REQUIRE(completed == 0);
    }

    // receive second request
    {
        brokerRun("GET /feed/aambertin HTTP/1.1\n"
                  "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
                  "Host: dev-challenges.kerno.io\n"
                  "Accept-Language: en-us\n"
                  "X-Trace-ID: AwUCAwEC\n"
                  " ");

        auto [pending, completed] = mapping->getStat();
        REQUIRE(pending == 2);
        REQUIRE(completed == 0);
    }

    // receive second response
    {
        brokerRun("HTTP/1.1 501 NOT IMPLEMENTED\n"
                  "Date: Thu Mar 14 2024 08:37:01 GMT+0000\n"
                  "Server: nginx\n"
                  "X-Trace-ID: AwUCAwEC\n"
                  "Connection: Closed\n"
                  " ");

        auto [pending, completed] = mapping->getStat();
        REQUIRE(pending == 1);
        REQUIRE(completed == 1);
        auto items = std::move(mapping->pickCompletedItems());
        REQUIRE(items.size() == 1);
        REQUIRE(items.front().id == "AwUCAwEC");
        REQUIRE(items.front().path == "/feed/aambertin");
        REQUIRE(items.front().code == "501");
    }

    // receive first response
    {
        brokerRun("HTTP/1.1 401 UNAUTHORIZED\n"
                  "Date: Thu Mar 14 2024 08:37:01 GMT+0000\n"
                  "Server: nginx\n"
                  "X-Trace-ID: BgYHAgYB\n"
                  "Connection: Closed\n"
                  " ");

        auto [pending, completed] = mapping->getStat();
        REQUIRE(pending == 0);
        REQUIRE(completed == 1);
        auto items = std::move(mapping->pickCompletedItems());
        REQUIRE(items.size() == 1);
        REQUIRE(items.front().id == "BgYHAgYB");
        REQUIRE(items.front().path == "/feed/smadigan");
        REQUIRE(items.front().code == "401");
    }
}
