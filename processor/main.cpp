#include <iostream>
#include "Broker.h"

int main() {
    std::string input;

    Broker broker;

    // 从标准输入读取数据，直到遇到 EOF
    while (std::getline(std::cin, input)) {
        // 将处理后的结果发送到标准输出
//        std::cout << input << std::endl;
        auto message = broker.handleInput(std::move(input));
        switch (message.messageType) {
            case none:
//                std::cout << input << std::endl;
                break;
            case request:
                std::cout << "request: " << message.id << " - " << message.path << std::endl;
                break;
            case response:
                std::cout << "response: " << message.id << " - " << message.code << std::endl;
                break;
        }
    }

    return 0;
}
