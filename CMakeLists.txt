cmake_minimum_required(VERSION 3.23)
project(bxt CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    libcoro
    GIT_REPOSITORY https://github.com/jbaldwin/libcoro.git
    GIT_TAG        v0.6
)
FetchContent_MakeAvailable(libcoro)

FetchContent_Declare(
    lmdbxx
    GIT_REPOSITORY https://github.com/hoytech/lmdbxx
    GIT_TAG        b12d1b12f4c9793aef8bed03d07ecbf789e810b4
)
FetchContent_MakeAvailable(lmdbxx)

FetchContent_Declare(
    event-bus
    GIT_REPOSITORY https://github.com/gelldur/EventBus.git
    GIT_TAG        v3.0.3
)

FetchContent_GetProperties(event-bus)
if(NOT event-bus_POPULATED)
    FetchContent_Populate(event-bus)
    add_subdirectory(${event-bus_SOURCE_DIR}/lib ${event-bus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif(NOT event-bus_POPULATED)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(daemon)
