# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (A101-GUI)

set(CMAKE_UNITY_BUILD OFF)

include_directories(
    .
    ../../Device/src
    ../../Device/src/Hardware/HAL
    ../../VS/ThirdParty/wxWidgets/include
    ../../generated/ThirdParty/lib/vc_dll/mswud
    ../../common/_VS_GUI/HAL
    ../../common/_VS_GUI/CMSIS
)

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

    add_compile_options(
        /wd4127    # conditional expression is constant
        /wd4464    # relative include path contains '..'
        /wd4514    # unreferenced inline function has been removed
#        /wd4623    # 'XXX': default constructor was implicitly defined as deleted
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
        /wd5204
        /wd5219    # implicit conversion from 'int' to 'float'
        /wd5264    # variable is not used
    )

endif()

file(GLOB SRC                   ./*.h
                                ./*.cpp
                                ../../Device/src/Settings.*
                                ../../Device/src/defines.h
                                ../../Device/src/stm_includes.h
                                ../../Device/src/Device.*)
file(GLOB CONTROLS                              Controls/*.*)
file(GLOB GENERATOR             ../../Device/src/Generator/*.*)
file(GLOB GUI                                  ./GUI/*.*)
file(GLOB HARDWARE              ../../Device/src/Hardware/*.*)
file(GLOB HARDWARE_HAL                         ./Hardware/HAL/*.*
                                ../../Device/src/Hardware/HAL/HAL.h
                                ../../Device/src/Hardware/HAL/HAL_PIO.h
                                ../../Device/src/Hardware/HAL/HAL_TIM.cpp)
file(GLOB AMPERMETER                             Ampermeter/*.*
                                ../../Device/src/Ampermeter/AD7691.*
                                ../../Device/src/Ampermeter/Ampermeter.*
                                ../../Device/src/Ampermeter/Ampermeter_settings.*
                                ../../Device/src/Ampermeter/BufferADC.*
                                ../../Device/src/Ampermeter/FIR.*
                                ../../Device/src/Ampermeter/InputRelays.*)
file(GLOB AMPERMETER_CALC       ../../Device/src/Ampermeter/Calculator/*.*)
file(GLOB AMPERMETER_CALIBRATOR ../../Device/src/Ampermeter/Calibrator/*.*)
file(GLOB COMMUNICATOR                           Communicator/*.*)
file(GLOB MENU                  ../../Device/src/Menu/*.*)
file(GLOB MENU_PAGES            ../../Device/src/Menu/Pages/*.*)
file(GLOB NEXTION                              ./Nextion/*.*
                                ../../Device/src/Nextion/*.h
                                ../../Device/src/Nextion/Diagram*.cpp
                                ../../Device/src/Nextion/Display.cpp
                                ../../Device/src/Nextion/Colors.cpp
                                ../../Device/src/Nextion/Keyboard.cpp
                                ../../Device/src/Nextion/Nextion.cpp)
file(GLOB NEXTION_CONTROLS      ../../Device/src/Nextion/Controls/*.*)
file(GLOB SCPI                  ../../Device/src/SCPI/*.*)
file(GLOB SETTINGS              ../../Device/src/Settings/*.*)
file(GLOB UTILS                 ../../Device/src/Utils/*.*)

source_group(src                   FILES ${SRC})
source_group(Generator             FILES ${GENERATOR})
source_group(GUI                   FILES ${GUI})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(Ampermeter            FILES ${AMPERMETER})
source_group(Ampermeter/Calculator FILES ${AMPERMETER_CALC})
source_group(Ampermeter/Calibrator FILES ${AMPERMETER_CALIBRATOR})
source_group(Communicator          FILES ${COMMUNICATOR})
source_group(Controls              FILES ${CONTROLS})
source_group(Menu                  FILES ${MENU})
source_group(Menu/Pages            FILES ${MENU_PAGES})
source_group(Nextion               FILES ${NEXTION})
source_group(Nextion/Controls      FILES ${NEXTION_CONTROLS})
source_group(SCPI                  FILES ${SCPI})
source_group(Settings              FILES ${SETTINGS})
source_group(Utils                 FILES ${UTILS})

add_executable(
    ${PROJECT_NAME}
    ${SRC}
    ${GENERATOR}
    ${GUI}
    ${HARDWARE}
    ${HARDWARE_HAL}
    ${AMPERMETER}
    ${AMPERMETER_CALC}
    ${AMPERMETER_CALIBRATOR}
    ${COMMUNICATOR}
    ${CONTROLS}
    ${MENU}
    ${MENU_PAGES}
    ${NEXTION}
    ${NEXTION_CONTROLS}
    ${SCPI}
    ${SETTINGS}
    ${UTILS}
)

target_link_libraries(${PROJECT_NAME} wxbase33ud wxmsw33ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw331ud_core_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase331ud_vc_custom.dll $(OutputPath) /Y)
