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

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(daemon)
