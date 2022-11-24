# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Loader)


set(CMAKE_UNITY_BUILD ON) 


include_directories(src
                    src/Hardware/HAL
                    ../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    )

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4127)    # conditional expression is constant
add_compile_options(/wd4464)    # relative include path contains '..'
add_compile_options(/wd4514)    # unreferenced inline function
add_compile_options(/wd4623)
add_compile_options(/wd4625)
add_compile_options(/wd4626)    # assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # 'x' bytes padding added after
add_compile_options(/wd5027)    # move assignment operator was implicitly defined as deleted
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation
add_compile_options(/wd5204)
add_compile_options(/wd5219)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies

file(GLOB LIBS_HAL         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                           ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)
file(GLOB SRC              src/*.*)
file(GLOB HARDWARE         src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS   src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL     src/Hardware/HAL/*.*)
file(GLOB SCPI             src/SCPI/*.*)
file(GLOB UTILS            src/Utils/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(src                   FILES ${SRC})
source_group(libs/HAL              FILES ${LIBS_HAL})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/CMSIS        FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(SCPI                  FILES ${SCPI})
source_group(Utils                 FILES ${UTILS})

add_library(${PROJECT_NAME} ${LIBS_HAL}
                            ${SRC}
                            ${HARDWARE}
                            ${HARDWARE_CMSIS}
                            ${HARDWARE_HAL}
                            ${SCPI}
                            ${UTILS}
)
