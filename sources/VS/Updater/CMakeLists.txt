# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by

project (Updater)

set(CMAKE_UNITY_BUILD ON)

include_directories(.)

if(WIN32)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
    
    # Older cmake versions don't support string(APPEND ...) provide a workaround
    macro(wx_string_append var str)
        set(${var} ${${var}}${str})
    endmacro()

    if(BUILD_USE_STATIC_RUNTIME)
        set(MSVC_LIB_USE "/MT")
        set(MSVC_LIB_REPLACE "/MD")
        
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

#    add_compile_options(/wd4365)    # 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#    add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
#    add_compile_options(/wd4625)    # 'derived class' : copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
#    add_compile_options(/wd4626)    # 'derived class' : assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
#    add_compile_options(/wd4820)    # 'bytes' bytes padding added after construct 'member_name'
    add_compile_options(/wd4996)    # Your code uses a function, class member, variable, or typedef that's marked deprecated
#    add_compile_options(/wd5026)    # move constructor was implicitly defined as deleted
#    add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
    add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C
                                    # function under -EHc.
    add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
    add_compile_options(/wd5264)    # variable is not used

endif()

file(GLOB SRC                    *.h *.c *.cpp *.me)

source_group(src                 FILES ${SRC})

add_executable( ${PROJECT_NAME}
                ${SRC}
                )

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "update101")

set_target_properties(Updater PROPERTIES
    VS_DEBUGGER_COMMAND_ARGUMENTS "com1 a101.bin")
