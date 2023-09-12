cmake_minimum_required(VERSION 3.22)
project(bxt CXX)

list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    lmdbxx
    GIT_REPOSITORY https://github.com/hoytech/lmdbxx
    GIT_TAG        b12d1b12f4c9793aef8bed03d07ecbf789e810b4
)
FetchContent_MakeAvailable(lmdbxx)

FetchContent_Declare(
    libcoro
    GIT_REPOSITORY https://github.com/jbaldwin/libcoro.git
    GIT_TAG        76ae314434a5d4ba9010628ea447746bd0a18806
)
FetchContent_MakeAvailable(libcoro)

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


if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
endif()

include("${CMAKE_BINARY_DIR}/conan.cmake")
conan_cmake_run(CONANFILE conanfile.txt
    BASIC_SETUP
    BUILD missing)

add_subdirectory(daemon)
add_subdirectory(frontend)
