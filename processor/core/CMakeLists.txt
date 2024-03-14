cmake_minimum_required(VERSION 3.27.8)
project(core)

set(CMAKE_CXX_STANDARD 26)

add_library(core STATIC
        Broker.cpp
        Broker.h
        Message.h
        Mapping.cpp
        Mapping.h
        Persistor.cpp
        Persistor.h
)
