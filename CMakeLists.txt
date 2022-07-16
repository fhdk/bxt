cmake_minimum_required(VERSION 3.16)
project(bxt)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


macro(setup_conan)
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
      message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
      file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                    "${CMAKE_BINARY_DIR}/conan.cmake"
                    TLS_VERIFY ON)
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_configure(CONANFILE conanfile.txt
                         BASIC_SETUP CMAKE_TARGETS
                         BUILD missing)

    conan_cmake_autodetect(settings)

    conan_cmake_install(PATH_OR_REFERENCE .
                        BUILD missing
                        REMOTE conancenter
                        SETTINGS ${settings})
endmacro(setup_conan)

setup_conan()

find_package(fmt)
find_package(Boost 1.66.0 REQUIRED COMPONENTS fiber date_time filesystem system thread log log_setup)
find_package(mailio)
find_package(frozen)
find_package(tomlplusplus REQUIRED)

add_subdirectory(daemon)
