//
// Created by serika on 2024/3/14.
//

#include "Mapping.h"
#include "Message.h"
#include <ranges>
#include <algorithm>

void Mapping::handleMessage(const Message &message) {
    switch (message.messageType) {
        case none:
            break;
        case request:
            addPendingItem(message.id, message.path);
            break;
        case response:
            completeItem(message.id, message.code);
            break;
    }
}

void Mapping::addPendingItem(const std::string &id, const std::string &path) {
    std::scoped_lock<std::mutex> lock(mutex);
    mapping[id] = MappingItem{
            id,
            path,
            "",
            MappingItem::State::pending
    };
}

void Mapping::completeItem(const std::string &id, const std::string &code) {
    std::scoped_lock<std::mutex> lock(mutex);
    auto iter = mapping.find(id);
    if (iter == mapping.end()) {
        return;
    }
    iter->second.code = code;
    iter->second.state = MappingItem::State::completed;
}

Mapping::Stat Mapping::getStat() {
    std::scoped_lock<std::mutex> lock(mutex);

    int pendingCount = 0;
    int completedCount = 0;
    for (const auto &[key, val]: mapping) {
        switch (val.state) {
            case MappingItem::pending:
                ++pendingCount;
                break;
            case MappingItem::completed:
                ++completedCount;
                break;
        }
    }
    return {pendingCount, completedCount};
}

MappingItems Mapping::pickCompletedItems() {
    std::scoped_lock<std::mutex> lock(mutex);

    MappingItems out;
    using Pair = std::pair<std::string, MappingItem>;
    auto isCompleted = [](const std::pair<std::string, MappingItem> &pair) {
        return pair.second.state == MappingItem::completed;
    };

    auto completed = mapping | std::views::filter(isCompleted);
    std::ranges::transform(completed, std::back_inserter(out), [](const auto &pair) {
        return pair.second;
    });

    erase_if(mapping, isCompleted);

    return out;
}
