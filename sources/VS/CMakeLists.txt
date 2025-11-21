# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

cmake_minimum_required(VERSION 3.15)

# Set project name
project (A101-GUI)

set (TARGET_NAME A101-GUI)

set(CMAKE_UNITY_BUILD ON)

if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
    add_definitions(-DDEBUG)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")         # Установка флага линковки /MT

add_compile_options(
    /wd4865 # '<unnamed-enum-XXX>': the underlying type will change from 'int' to '__int64' when '/Zc:enumTypes' is specified on the command line
)

add_subdirectory(ControlPanel)
add_subdirectory(A101-GUI)
#add_subdirectory(Explorer)
add_subdirectory(Bootloader)
