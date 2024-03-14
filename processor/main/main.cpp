#include <iostream>
#include "../core/Broker.h"
#include "../core/Mapping.h"
#include "../core/Persistor.h"
#include "Processor.h"
#include <memory>

void printHelpMessage() {
    auto helpMessage = "usage: /out/bin/generator | /out/bin/processor -o <filepath>";
    std::cerr << helpMessage << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 3 ||
        (std::string(argv[1]) != std::string("-o"))) {
        printHelpMessage();
        return 1;
    }

    std::string saveTo = argv[2];
    if (saveTo.empty()) {
        printHelpMessage();
        return 1;
    }

    auto mapping = std::make_shared<Mapping>();
    auto broker = std::make_unique<Broker>(std::cin, mapping);
    auto persistor = std::make_unique<Persistor>(std::move(saveTo), mapping);

    auto processor = std::make_unique<Processor>(std::move(broker), std::move(persistor));

    processor->

            run();

    return 0;
}
