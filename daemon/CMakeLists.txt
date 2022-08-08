cmake_minimum_required(VERSION 3.16)
project(bxtd LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost COMPONENTS system iostreams date_time regex REQUIRED)
find_package(mailio REQUIRED)
find_package(frozen REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(LibArchive REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(phmap REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC -fopenmp -DBOOST_LOG_DYN_LINK -DBOOST")


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

include_directories(
    ./
    ${CURL_INCLUDE_DIR}
)

add_compile_definitions(${PROJECT_NAME} PUBLIC BOOST_ASIO_HAS_CO_AWAIT=1 BOOST_ASIO_HAS_STD_COROUTINE=1)
target_compile_options(${PROJECT_NAME} PUBLIC -fcoroutines)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    fmt::fmt
    Boost::boost
    Boost::iostreams
    mailio::mailio
    frozen::frozen
    tomlplusplus::tomlplusplus
    LibArchive::LibArchive
    OpenSSL::Crypto
    phmap
)


