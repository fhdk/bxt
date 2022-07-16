cmake_minimum_required(VERSION 3.16)
project(bxtd LANGUAGES CXX)

find_package(fmt REQUIRED)
find_package(CURL REQUIRED)
find_package(Boost 1.66.0 REQUIRED COMPONENTS fiber date_time filesystem system thread log log_setup)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SIGCXX REQUIRED sigc++-3.0)

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
    fmt::fmt
    Boost::date_time 
    Boost::filesystem 
    Boost::system 
    Boost::thread 
    Boost::log 
    Boost::log_setup
    Boost::fiber
    tomlplusplus::tomlplusplus
)


