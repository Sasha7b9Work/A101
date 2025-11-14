# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (ControlPanel)

#set(CMAKE_UNITY_BUILD OFF)

include_directories(
    .
    ../../VS/ThirdParty/wxWidgets/include
    ../../generated/ThirdParty/lib/vc_lib/mswu
)

link_directories(../../generated/ThirdParty/lib/vc_lib)

add_definitions(-DSTM32F103xB -DGUI -DDEVICE)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    
#    add_compile_options(/wd4244)    # initializing': conversion from 'double' to 'int', possible loss of data
    add_compile_options(
        /wd4464    # relative include path contains '..'
        /wd4514    # unreferenced inline function has been removed
        /wd4625    # copy constructor was implicitly defined as deleted
        /wd4626    # assignment operator was implicitly defined as deleted
        /wd4710
        /wd4711
        /wd4774    # 'string' : format string expected in argument number is not a string literal
        /wd4820    # 'bytes' bytes padding added after construct 'member_name'
        /wd4996    # This function or variable may be unsafe
        /wd5026    # move constructor was implicitly defined as deleted
        /wd5027    # move assignment operator was implicitly defined as deleted
        /wd5039    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
        /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specifie
        /wd5219    # implicit conversion from 'int' to 'float'
        /wd5264    # variable is not used
    )
endif()

file(GLOB SRC               *.h  *.cpp *.c)
file(GLOB DIALOGS           Dialogs/*.*)
file(GLOB DISPLAY           ../../Device/src/Nextion/Colors.*)
file(GLOB SCPI              SCPI/*.*)
file(GLOB SCPI_PARSER       SCPI/Parser/*.*)

source_group(src            FILES ${SRC})
source_group(Dialogs        FILES ${DIALOGS})
source_group(Display        FILES ${DISPLAY})
source_group(SCPI           FILES ${SCPI})
source_group(SCPI/Parser    FILES ${SCPI_PARSER})

add_executable(
    ${PROJECT_NAME}
    ${SRC}
    ${DIALOGS}
    ${DISPLAY}
    ${SCPI}
    ${SCPI_PARSER}
)

target_link_libraries(${PROJECT_NAME}
    wxbase33u
    wxmsw33u_core
    wxpng
    wxzlib
    Ws2_32
    Comctl32
    Rpcrt4
    Setupapi
)
