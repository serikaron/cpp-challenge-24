//
// Created by serika on 2024/3/13.
//

#ifndef PROCESSOR_BROKER_H
#define PROCESSOR_BROKER_H

#include <vector>
#include <string>
#include "Message.h"

class Broker {
public:
    Message handleInput(std::string &&line);

private:
    [[nodiscard]] bool isRequestBlock() const;
    [[nodiscard]] bool isResponseBlock() const;
    [[nodiscard]] std::string getPath() const;
    [[nodiscard]] std::string getCode() const;
    [[nodiscard]] std::string getID() const;

private:
    using Block = std::vector<std::string>;
    Block block;
};


#endif //PROCESSOR_BROKER_H
