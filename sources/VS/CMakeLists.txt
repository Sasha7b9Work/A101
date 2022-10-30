# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

cmake_minimum_required(VERSION 3.13)

# Set project name
project (A101-GUI)

set (TARGET_NAME A101-GUI)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(Device)
add_subdirectory(Updater)
