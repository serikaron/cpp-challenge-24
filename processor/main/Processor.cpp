//
// Created by serika on 2024/3/14.
//

#include "Processor.h"
#include <thread>
#include <chrono>

Processor::Processor(std::unique_ptr<Broker> broker, std::unique_ptr<Persistor> persistor)
        : broker(std::move(broker)), persistor(std::move(persistor)) {
}

void Processor::run() {
    std::thread brokerT{[b = broker.get()] {
        b->run();
    }};

    std::thread persistorT{[p = persistor.get(), flag = &running] {
        using namespace std::chrono_literals;

        while (flag->isRunning()) {
            std::this_thread::sleep_for(1m);
            p->save();
        }
    }};

    brokerT.join();
    running.terminate();
    persistorT.join();
}

void Processor::Running::terminate() {
    std::scoped_lock<std::mutex> lock(mutex);
    running = false;
}

bool Processor::Running::isRunning() {
    std::scoped_lock<std::mutex> lock(mutex);
    return running;
}
