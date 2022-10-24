# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Device)


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

add_compile_options(/wd4505)
add_compile_options(/wd4514)
add_compile_options(/wd4820)
add_compile_options(/wd5045)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies

file(GLOB LIBS_HAL       ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)
#file(GLOB LIBS_HAL_VS    ../common/_VS_GUI/HAL/stm32f4xx_hal_rcc.cpp
#                         ../common/_VS_GUI/HAL/stm32f4xx_hal_cortex.cpp
#                         ../common/_VS_GUI/HAL/stm32f4xx_hal.cpp)
file(GLOB SRC            src/*.*)
file(GLOB DISPLAY        src/Display/*.*)
file(GLOB HARDWARE       src/Hardware/*.*)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*.*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*.*)
file(GLOB MEASURER       src/Measurer/*.*)
file(GLOB MENU           src/Menu/*.*)
file(GLOB MENU_PAGES     src/Menu/Pages/*.*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(src             FILES ${SRC})
source_group(Display         FILES ${DISPLAY})
source_group(libs/HAL        FILES ${LIBS_HAL})
#source_group(libs/HAL_VS     FILES ${LIBS_HAL_VS})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware/CMSIS  FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL    FILES ${HARDWARE_HAL})
source_group(Measurer        FILES ${MEASURER})
source_group(Menu            FILES ${MENU})
source_group(Menu/Pages      FILES ${MENU_PAGES})

add_library(${PROJECT_NAME} ${LIBS_HAL}
#                               ${LIBS_HAL_VS}
                            ${SRC}
                            ${DISPLAY}
                            ${HARDWARE}
                            ${HARDWARE_CMSIS}
                            ${HARDWARE_HAL}
                            ${MEASURER}
                            ${MENU}
                            ${MENU_PAGES}
)
