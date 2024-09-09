# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Device)


set(CMAKE_UNITY_BUILD ON)


include_directories(
    src
    src/Hardware/HAL
    ../common/_VS_KEIL
    ../common/_VS_KEIL/CMSIS
    ../common/_VS_KEIL/std_includes
    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
)

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(
    /wd4127    # conditional expression is constant
    /wd4464    # relative include path contains '..'
    /wd4514    # unreferenced inline function
    /wd4623
    /wd4625
    /wd4626    # assignment operator was implicitly defined as deleted
    /wd4710
    /wd4820    # 'x' bytes padding added after
    /wd5027    # move assignment operator was implicitly defined as deleted
    /wd5045    # Compiler will insert Spectre mitigation
    /wd5204
    /wd5219
    /wd5220    # a non-static data member with a volatile qualified type no longer implies
)

file(GLOB LIBS_HAL              ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                                ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)
file(GLOB SRC                   src/*.*)
file(GLOB AMPERMETER            src/Ampermeter/*.*)
file(GLOB AMPERMETER_CALC       src/Ampermeter/Calculator/*.*)
file(GLOB AMPERMETER_CALIBRATOR src/Ampermeter/Calibrator/*.*)
file(GLOB HARDWARE              src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS        src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL          src/Hardware/HAL/*.*)
file(GLOB MENU                  src/Menu/*.*)
file(GLOB MENU_PAGES            src/Menu/Pages/*.*)
file(GLOB NEXTION               src/Nextion/*.*)
file(GLOB NEXTION_CONTROLS      src/Nextion/Controls/*.*)
file(GLOB SCPI                  src/SCPI/*.*)
file(GLOB SETTINGS              src/Settings/*.*)
file(GLOB UTILS                 src/Utils/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(src                   FILES ${SRC})
source_group(Ampermeter            FILES ${AMPERMETER})
source_group(Ampermeter/Calculator FILES ${AMPERMETER_CALC})
source_group(Ampermeter/Calibrator FILES ${AMPERMETER_CALIBRATOR})
source_group(libs/HAL              FILES ${LIBS_HAL})
source_group(Hardware              FILES ${HARDWARE})
source_group(Hardware/CMSIS        FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL          FILES ${HARDWARE_HAL})
source_group(Menu                  FILES ${MENU})
source_group(Menu/Pages            FILES ${MENU_PAGES})
source_group(Nextion               FILES ${NEXTION})
source_group(Nextion/Controls      FILES ${NEXTION_CONTROLS})
source_group(SCPI                  FILES ${SCPI})
source_group(Settings              FILES ${SETTINGS})
source_group(Utils                 FILES ${UTILS})

add_library(
    ${PROJECT_NAME}
    ${LIBS_HAL}
    ${SRC}
    ${AMPERMETER}
    ${AMPERMETER_CALC}
    ${AMPERMETER_CALIBRATOR}
    ${HARDWARE}
    ${HARDWARE_CMSIS}
    ${HARDWARE_HAL}
    ${MENU}
    ${MENU_PAGES}
    ${NEXTION}
    ${NEXTION_CONTROLS}
    ${SCPI}
    ${SETTINGS}
    ${UTILS}
)
