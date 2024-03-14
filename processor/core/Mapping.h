//
// Created by serika on 2024/3/14.
//

#ifndef PROCESSOR_MAPPING_H
#define PROCESSOR_MAPPING_H

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <mutex>

struct Message;

struct MappingItem {
    enum State {pending, completed};

    std::string id;
    std::string path;
    std::string code;
    State state;
};
using MappingItems = std::vector<MappingItem>;

class Mapping {
public:
    using Stat = std::tuple<int, int>;
    void handleMessage(const Message& message);
    [[nodiscard]] Stat getStat();
    MappingItems pickCompletedItems();

private:
    void addPendingItem(const std::string &id, const std::string& path);
    void completeItem(const std::string &id, const std::string &code);

private:
    std::mutex mutex;
    using Map = std::map<std::string, MappingItem>;
    Map mapping;
};

using SharedMapping = std::shared_ptr<Mapping>;

#endif //PROCESSOR_MAPPING_H
