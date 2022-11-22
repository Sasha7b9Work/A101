# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Device)


set(CMAKE_UNITY_BUILD OFF)


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
add_compile_options(/wd4820)    # 'x' bytes padding added after
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation
add_compile_options(/wd5219)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies

file(GLOB LIBS_HAL         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                           ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)
file(GLOB SRC              src/*.*)
file(GLOB DISPLAY          src/Display/*.*)
file(GLOB DISPLAY_CONTROLS src/Display/Controls/*.*)
file(GLOB GENERATOR        src/Generator/*.*)
file(GLOB HARDWARE         src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS   src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL     src/Hardware/HAL/*.*)
file(GLOB AMPERMETER       src/Ampermeter/*.*)
file(GLOB AMPERMETER_CALC  src/Ampermeter/Calculator/*.*)
file(GLOB MENU             src/Menu/*.*)
file(GLOB MENU_PAGES       src/Menu/Pages/*.*)
file(GLOB NEXTION          src/Nextion/*.*)
file(GLOB SCPI             src/SCPI/*.*)
file(GLOB SCPI_PARSER      src/SCPI/Parser/*.*)
file(GLOB SETTINGS         src/Settings/*.*)
file(GLOB UTILS            src/Utils/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(src                   FILES ${SRC})
source_group(Display               FILES ${DISPLAY})
source_group(Display/Controls      FILES ${DISPLAY_CONTROLS})
source_group(Generator             FILES ${GENERATOR})
source_group(libs/HAL              FILES ${LIBS_HAL})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/CMSIS        FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(Ampermeter            FILES ${AMPERMETER})
source_group(Ampermeter/Calculator FILES ${AMPERMETER_CALC})
source_group(Menu                  FILES ${MENU})
source_group(Menu/Pages            FILES ${MENU_PAGES})
source_group(Nextion               FILES ${NEXTION})
source_group(SCPI                  FILES ${SCPI})
source_group(SCPI/Parser           FILES ${SCPI_PARSER})
source_group(Settings              FILES ${SETTINGS})
source_group(Utils                 FILES ${UTILS})

add_library(${PROJECT_NAME} ${LIBS_HAL}
                            ${SRC}
                            ${DISPLAY}
                            ${DISPLAY_CONTROLS}
                            ${GENERATOR}
                            ${HARDWARE}
                            ${HARDWARE_CMSIS}
                            ${HARDWARE_HAL}
                            ${AMPERMETER}
                            ${AMPERMETER_CALC}
                            ${MENU}
                            ${MENU_PAGES}
                            ${NEXTION}
                            ${SCPI}
                            ${SCPI_PARSER}
                            ${SETTINGS}
                            ${UTILS}
)
