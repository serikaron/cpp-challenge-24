//
// Created by serika on 2024/3/13.
//

#ifndef PROCESSOR_BROKER_H
#define PROCESSOR_BROKER_H

#include <vector>
#include <string>
#include <istream>
#include "Message.h"
#include "Mapping.h"

class Mapping;

class Broker {
public:
    Broker(std::istream &stream, SharedMapping mapping);
    Broker(const Broker &) = delete;
    Broker& operator=(const Broker &) = delete;

public:
    void run();

private:
    Message handleInput(std::string &&line);
    [[nodiscard]] bool isRequestBlock() const;
    [[nodiscard]] bool isResponseBlock() const;
    [[nodiscard]] std::string getPath() const;
    [[nodiscard]] std::string getCode() const;
    [[nodiscard]] std::string getID() const;

private:
    std::istream &stream;
    using Block = std::vector<std::string>;
    Block block;
    SharedMapping mapping;
};


#endif //PROCESSOR_BROKER_H
