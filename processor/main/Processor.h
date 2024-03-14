//
// Created by serika on 2024/3/14.
//

#ifndef PROCESSOR_PROCESSOR_H
#define PROCESSOR_PROCESSOR_H

#include "../core/Broker.h"
#include "../core/Persistor.h"
#include <memory>
#include <mutex>

class Processor {
public:
    Processor(std::unique_ptr<Broker> broker, std::unique_ptr<Persistor> persistor);
    Processor(const Processor&) = delete;
    Processor& operator=(const Processor&) = delete;

    void run();

private:
    class Running {
    public:
        void terminate();
        bool isRunning();
    private:
        std::mutex mutex;
        bool running = true;
    };

private:
    std::unique_ptr<Broker> broker;
    std::unique_ptr<Persistor> persistor;
    Running running;
};


#endif //PROCESSOR_PROCESSOR_H
