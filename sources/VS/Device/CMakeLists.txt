# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Device)

set(CMAKE_UNITY_BUILD ON)

include_directories(../../Device/src
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

    add_compile_options(/wd4005)
    add_compile_options(/wd4100)
    add_compile_options(/wd4255)
    add_compile_options(/wd4365)
    add_compile_options(/wd4464)
    add_compile_options(/wd4548)
    add_compile_options(/wd4514)
    add_compile_options(/wd4623)
    add_compile_options(/wd4625)
    add_compile_options(/wd4626)
    add_compile_options(/wd4774)
    add_compile_options(/wd4820)
    add_compile_options(/wd4996)
    add_compile_options(/wd5026)
    add_compile_options(/wd5027)
    add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                    # function under -EHc.
    add_compile_options(/wd5045)
    add_compile_options(/wd5220)
    add_compile_options(/wd26812)
endif()

file(GLOB SRC                   ./*.h
                                ./*.cpp
                                ../../Device/src/defines.h
                                ../../Device/src/stm_includes.h)
file(GLOB DISPLAY               ../../Device/src/Display/*.*)
file(GLOB HARDWARE              ../../Device/src/Hardware/*.*)
file(GLOB HARDWARE_HAL          ./Hardware/HAL/*.*
                                ../../Device/src/Hardware/HAL/HAL.h
                                ../../Device/src/Hardware/HAL/HAL_PIO.h
                                ../../Device/src/Hardware/HAL/HAL_TIM.cpp)
file(GLOB MEASURER              ../../Device/src/Measurer/*.*)
file(GLOB MENU                  ../../Device/src/Menu/*.*)
file(GLOB MENU_PAGES            ../../Device/src/Menu/Pages/*.*)
file(GLOB UTILS                 ../../Device/src/Utils/*.*)

source_group(src                FILES ${SRC})
source_group(Display            FILES ${DISPLAY})
source_group(Hardware           FILES ${HARDWARE})
source_group(Hardware/HAL       FILES ${HARDWARE_HAL})
source_group(Measurer           FILES ${MEASURER})
source_group(Menu               FILES ${MENU})
source_group(Menu/Pages         FILES ${MENU_PAGES})
source_group(Utils              FILES ${UTILS})

add_executable( ${PROJECT_NAME}
                ${SRC}
                ${DISPLAY}
                ${HARDWARE}
                ${HARDWARE_HAL}
                ${MEASURER}
                ${MENU}
                ${MENU_PAGES}
                ${UTILS}
                )

target_link_libraries(${PROJECT_NAME} wxbase33ud wxmsw33ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw330ud_core_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase330ud_vc_custom.dll $(OutputPath) /Y)
