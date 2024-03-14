//
// Created by serika on 2024/3/14.
//

#include "Persistor.h"
#include "Mapping.h"
#include <iomanip>
#include <ctime>
#include <sstream>
#include <ranges>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <iostream>

Persistor::Persistor(std::string &&saveTo,
                     SharedMapping mapping
) : saveTo(std::move(saveTo)), mapping(mapping) {
}

void Persistor::save() {
    std::cout << "save" << std::endl;
    save(std::move(genContent()));
}

Persistor::Content Persistor::genContent() {
    auto now = [] {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%F %T");
        return ss.str();
    };

    const auto [pendingCount, completedCount] = mapping->getStat();
    auto statRow = [pendingCount, completedCount] {
        std::stringstream ss;
        ss << "pending," << pendingCount << ",completed," << completedCount;
        return ss.str();
    };

    auto header = "path,code";
    Content out{
            now(),
            statRow(),
            header,
    };

    if (completedCount > 0) {
        auto items = std::move(mapping->pickCompletedItems());
        auto transform = [](const MappingItem &item) {
            std::stringstream ss;
            ss << item.path << "," << item.code;
            return ss.str();
        };
        auto rows = items | std::views::transform(transform);
        out.insert(out.end(), rows.begin(), rows.end());
    }

    return out;
}

void Persistor::save(Persistor::Content &&content) {
    std::filesystem::create_directories(std::filesystem::path(saveTo).parent_path());
    std::ofstream file(saveTo, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERROR: open file failed!!! [" << saveTo << "]" << std::endl;
        return;
    }

    for (const auto &line : content) {
        file << line << "\n";
    }

    file.close();
}
