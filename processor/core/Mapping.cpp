//
// Created by serika on 2024/3/14.
//

#include "Mapping.h"
#include "Message.h"
#include <ranges>
#include <functional>

void Mapping::handleMessage(const Message &message) {
    switch (message.messageType) {
        case none:
            break;
        case request:
            mapping[message.id] = MappingItem{
                    message.id,
                    message.path,
                    message.code,
                    MappingItem::State::pending
            };
            break;
        case response:
            auto iter = mapping.find(message.id);
            if (iter != mapping.end()) {
                iter->second.code = message.code;
                iter->second.state = MappingItem::State::completed;
            }
            break;
    }
}

Mapping::Stat Mapping::getStat() const {
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
    MappingItems out;
    using Pair = std::pair<std::string, MappingItem>;
    auto isCompleted = [](const std::pair<std::string, MappingItem> &pair) {
        return pair.second.state == MappingItem::completed;
    };

    auto completed = mapping | std::views::filter(isCompleted);
    std::ranges::transform(completed, std::back_inserter(out), [](const auto& pair) {
        return pair.second;
    });

    erase_if(mapping, isCompleted);

    return out;
}
