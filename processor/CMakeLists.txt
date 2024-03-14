cmake_minimum_required(VERSION 3.27.8)
project(processor)

set(CMAKE_CXX_STANDARD 26)

add_subdirectory(core)
add_subdirectory(main)

enable_testing()
add_subdirectory(test)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/out"
            CACHE PATH "default install path" FORCE)
endif ()

install(TARGETS processor core_tests)
