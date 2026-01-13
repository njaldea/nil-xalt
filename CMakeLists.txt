cmake_minimum_required(VERSION 3.27.1)

project(
    nil-xalt
    VERSION 1.4.1
    LANGUAGES CXX
    DESCRIPTION "collection of utility functionalities"
)

find_package(nil-cmake CONFIG REQUIRED)
install_config("cmake/config.cmake.in")

add_subdirectory(src)
add_subdirectory(sandbox)