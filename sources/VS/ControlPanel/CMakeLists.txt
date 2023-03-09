# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (ControlPanel)

set(CMAKE_UNITY_BUILD ON)

include_directories(.
                    ../../VS/ThirdParty/wxWidgets/include
                    ../../generated/ThirdParty/lib/vc_dll/mswud)

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
    add_compile_options(/wd4464)    # relative include path contains '..'
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
    add_compile_options(/wd5264)    # variable is not used
endif()

file(GLOB SRC               *.h  *.cpp *.c)
file(GLOB DIALOGS           Dialogs/*.*)
file(GLOB DISPLAY           ../../Device/src/Display/Colors.*)
file(GLOB SCPI              SCPI/*.*)
file(GLOB SCPI_PARSER       SCPI/Parser/*.*)

source_group(src            FILES ${SRC})
source_group(Dialogs        FILES ${DIALOGS})
source_group(Display        FILES ${DISPLAY})
source_group(SCPI           FILES ${SCPI})
source_group(SCPI/Parser    FILES ${SCPI_PARSER})

add_executable( ${PROJECT_NAME}
                ${SRC}
                ${DIALOGS}
                ${DISPLAY}
                ${SCPI}
                ${SCPI_PARSER}
                )

target_link_libraries(${PROJECT_NAME} wxbase32ud wxmsw32ud_core Ws2_32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxmsw32ud_core_vc_custom.dll $(OutputPath) /Y)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy ..\\..\\ThirdParty\\lib\\vc_dll\\wxbase32ud_vc_custom.dll $(OutputPath) /Y)
