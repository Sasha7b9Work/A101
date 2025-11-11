# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

cmake_minimum_required(VERSION 3.13)

# Set project name
project (A101-GUI)

set (TARGET_NAME A101-GUI)

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

add_subdirectory(ControlPanel)
add_subdirectory(A101-GUI)
#add_subdirectory(Explorer)
add_subdirectory(Bootloader)
add_subdirectory(hex2binA101)
