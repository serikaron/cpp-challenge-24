#include <iostream>

int main() {
    std::string input;

    // 从标准输入读取数据，直到遇到 EOF
    while (std::getline(std::cin, input)) {
        // 将处理后的结果发送到标准输出
        std::cout << input << std::endl;
    }

    return 0;
}
