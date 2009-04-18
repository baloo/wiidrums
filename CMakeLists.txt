CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(WIIDRUM) 

## cmake includes and plugins
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

FIND_PACKAGE(Qt4 REQUIRED)

## Just include the subdirs
ADD_SUBDIRECTORY(src)



