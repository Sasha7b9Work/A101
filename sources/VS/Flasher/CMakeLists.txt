# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Flasher)

#set(CMAKE_UNITY_BUILD OFF)

#set(CMAKE_UNITY_BUILD_BATCH_SIZE 8)

include_directories(
    src
    .
    lib/mfproto
    lib/serial/include
    binarylibs/src/libusb/msvc
    binarylibs/src/libusb/libusb
    ../../VS/ThirdParty/wxWidgets/include
    ../../generated/ThirdParty/lib/vc_dll/mswud
)

link_directories(
    ../../generated/ThirdParty/lib/vc_dll
)

add_definitions(-DWXUSINGDLL)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MTd -std:c++17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

    # Older cmake versions don't support string(APPEND ...) provide a workaround
    macro(wx_string_append var str)
        set(${var} ${${var}}${str})
    endmacro()
    
    if(BUILD_USE_STATIC_RUNTIME)
        set(MSVC_LIB_USE "/MTd")
        set(MSVC_LIB_REPLACE "/MDd")
        
        # Set MSVC runtime flags for all configurations
        foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
            set(c_flag_var CMAKE_C_FLAGS)
            set(cxx_flag_var CMAKE_CXX_FLAGS)
            if(cfg)
                string(TOUPPER ${cfg} cfg_upper)
                wx_string_append(c_flag_var "_${cfg_upper}")
                wx_string_append(cxx_flag_var "_${cfg_upper}")
            endif()
            if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
                string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
                set(${c_flag_var} ${${c_flag_var}} CACHE STRING
                "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
            endif()
            if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
                string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
                set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
                "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
            endif()
        endforeach()
    endif()
    
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


file(GLOB SRC         src/*.h src/*.cpp)
file(GLOB CONTROLS    Controls/*.h Controls/*.cpp)

source_group(src      FILES ${SRC})
source_group(Controls FILES ${CONTROLS})

add_executable( ${PROJECT_NAME}
    ${SRC}
    ${CONTROLS}
)

target_link_libraries(${PROJECT_NAME}
    wxbase33ud wxmsw33ud_core wxbase33ud_net wxzlibd wxpngd
    Ws2_32 Comctl32 Rpcrt4 Setupapi
)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw331ud_core_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase331ud_vc_custom.dll $(OutputPath) /Y)
