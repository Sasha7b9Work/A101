# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Device)

set(CMAKE_UNITY_BUILD ON)

include_directories(.
                    ../../Device/src
                    ../../Device/src/Hardware/HAL
                    ../../VS/ThirdParty/wxWidgets/include
                    ../../generated/ThirdParty/lib/vc_dll/mswud
                    ../../common/_VS_GUI/HAL
                    ../../common/_VS_GUI/CMSIS)

link_directories(../../generated/ThirdParty/lib/vc_dll)

add_definitions(-DSTM32F103xB -DWXUSINGDLL -DGUI -DDEVICE)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")
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

#    add_compile_options(/wd4244)    # initializing': conversion from 'double' to 'int', possible loss of data
    add_compile_options(/wd4514)    # unreferenced inline function has been removed
    add_compile_options(/wd4625)    # copy constructor was implicitly defined as deleted
    add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
    add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
    add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
    add_compile_options(/wd4996)    # This function or variable may be unsafe
    add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
    add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
    add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
    add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specifie
    add_compile_options(/wd5219)    # implicit conversion from 'int' to 'float'
endif()

file(GLOB SRC                   ./*.h
                                ./*.cpp
                                ../../Device/src/defines.h
                                ../../Device/src/stm_includes.h)
file(GLOB DISPLAY               ./Display/*.*
                                ../../Device/src/Display/*.h
                                ../../Device/src/Display/Colors.cpp
                                ../../Device/src/Display/DiagramFFT.cpp
                                ../../Device/src/Display/DiagramInput.cpp
                                ../../Device/src/Display/Display.cpp
                                ../../Device/src/Display/Indicator.cpp)
file(GLOB GENERATOR             ../../Device/src/Generator/*.*)
file(GLOB HARDWARE              ../../Device/src/Hardware/*.*)
file(GLOB HARDWARE_HAL          ./Hardware/HAL/*.*
                                ../../Device/src/Hardware/HAL/HAL.h
                                ../../Device/src/Hardware/HAL/HAL_PIO.h
                                ../../Device/src/Hardware/HAL/HAL_TIM.cpp)
file(GLOB AMPERMETER            ../../Device/src/Ampermeter/*.*)
file(GLOB AMPERMETER_CALC       ../../Device/src/Ampermeter/Calculator/*.*)
file(GLOB MENU                  ../../Device/src/Menu/*.*)
file(GLOB MENU_PAGES            ../../Device/src/Menu/Pages/*.*)
file(GLOB NEXTION               ./Nextion/*.*
                                ../../Device/src/Nextion/*.h
                                ../../Device/src/Nextion/Nextion.cpp)
file(GLOB UTILS                 ../../Device/src/Utils/*.*)

source_group(src                   FILES ${SRC})
source_group(Display               FILES ${DISPLAY})
source_group(Generator             FILES ${GENERATOR})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(Ampermeter            FILES ${AMPERMETER})
source_group(Ampermeter/Calculator FILES ${AMPERMETER_CALC})
source_group(Menu                  FILES ${MENU})
source_group(Menu/Pages            FILES ${MENU_PAGES})
source_group(Nextion               FILES ${NEXTION})
source_group(Utils                 FILES ${UTILS})

add_executable( ${PROJECT_NAME}
                ${SRC}
                ${DISPLAY}
                ${GENERATOR}
                ${HARDWARE}
                ${HARDWARE_HAL}
                ${AMPERMETER}
                ${AMPERMETER_CALC}
                ${MENU}
                ${MENU_PAGES}
                ${NEXTION}
                ${UTILS}
                )

target_link_libraries(${PROJECT_NAME} wxbase33ud wxmsw33ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw330ud_core_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase330ud_vc_custom.dll $(OutputPath) /Y)
