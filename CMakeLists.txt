
cmake_minimum_required(VERSION 2.6)

project(wiidrums)

## cmake includes and plugins
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}"
                      "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(Qt4 REQUIRED)

## Just include the subdirs
add_subdirectory(src)



