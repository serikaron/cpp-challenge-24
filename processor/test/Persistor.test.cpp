//
// Created by serika on 2024/3/14.
//

#include "catch2/catch_test_macros.hpp"
#include "../core/Persistor.h"
#include "../core/Message.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>

SCENARIO("Persistor test", "[Persistor]") {
    std::shared_ptr<Mapping> mapping = std::make_shared<Mapping>();

    auto saveTo = std::getenv("SAVE_TO");
    Persistor persistor{
            saveTo,
            mapping,
    };

    GIVEN("pending and completed items") {
        mapping->handleMessage(Message::request("1", "/completed/path"));
        mapping->handleMessage(Message::response("1", "200"));
        mapping->handleMessage(Message::request("2", "/pending/path"));

        WHEN("save") {
            persistor.save();

            THEN("saved file should be corrected") {
                std::vector<std::string> expectLines = {
                        "pending,1,completed,1",
                        "path,code",
                        "/completed/path,200"
                };

                std::ifstream file(saveTo);
                std::string line;
                int lineCount = -1; // -1 means skip the first line (time string)
                while (std::getline(file, line)) {
                    auto idx = lineCount++;
                    if (idx < 0) {
                        continue;
                    }
                    REQUIRE(line == expectLines[idx]);
                }
                REQUIRE(lineCount==3);
            }
        }
    }

    mapping.reset();
}