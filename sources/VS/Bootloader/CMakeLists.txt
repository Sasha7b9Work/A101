# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (BootA101)

set(CMAKE_UNITY_BUILD OFF)

#set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

include_directories(
    src
    .
    lib/mfproto
    lib/serial/include
    binarylibs/src/libusb/msvc
    binarylibs/src/libusb/libusb
    ../../VS/ThirdParty/wxWidgets/include
    ../../generated/ThirdParty/lib/vc_lib/mswu
)

link_directories(
    ../../generated/ThirdParty/lib/vc_lib
)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -std:c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

    add_compile_options(
        -DFLASHER
        /wd4018
        /wd4061
        /wd4100
        /wd4101
        /wd4189
        /wd4191
        /wd4200
        /wd4201
        /wd4242
        /wd4244
        /wd4265
        /wd4296
        /wd4365    # 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
        /wd4456
        /wd4458
        /wd4514    # 'function' : unreferenced inline function has been removed
        /wd4623
        /wd4625    # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
        /wd4626    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
        /wd4668
        /wd4702
        /wd4710
        /wd4711
        /wd4820    # 'bytes' bytes padding added after construct 'member_name'
        /wd4826
        /wd4996    # Your code uses a function, class member, variable, or typedef that's marked deprecated
        /wd5026    # move constructor was implicitly defined as deleted
        /wd5027    # move assignment operator was implicitly defined as deleted
        /wd5039    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                    # function under -EHc.
        /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5208
    )
endif()


file(GLOB SRC          *.h *.cpp)
file(GLOB COMMUNICATOR Communicator/*.*)
file(GLOB CONTROLS     Controls/*.*)
file(GLOB UTILS        Utils/*.*)

source_group(src          FILES ${SRC})
source_group(Communicator FILES ${COMMUNICATOR})
source_group(Controls     FILES ${CONTROLS})
source_group(Utils        FILES ${UTILS})

add_executable( ${PROJECT_NAME}
    ${SRC}
    ${COMMUNICATOR}
    ${CONTROLS}
    ${UTILS}
)

target_link_libraries(${PROJECT_NAME}
    wxbase33u
    wxmsw33u_core
    wxbase33u_net
    wxzlib
    wxpng
    Ws2_32
    Comctl32
    Rpcrt4
    Setupapi
)
