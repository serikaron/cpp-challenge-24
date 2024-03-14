cmake_minimum_required(VERSION 3.27.8)
project(processor)

set(CMAKE_CXX_STANDARD 26)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.3)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

add_executable(core_tests
        Broker.test.cpp
        Persistor.test.cpp
)
target_link_libraries(core_tests PRIVATE Catch2::Catch2WithMain core)
catch_discover_tests(core_tests)