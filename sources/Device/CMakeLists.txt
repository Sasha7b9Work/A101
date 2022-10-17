# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Device)


include_directories(src
                    ../common/_VS_KEIL
                    ../common/_VS_KEIL/CMSIS
                    ../common/_VS_KEIL/std_includes
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc

#                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Include
#                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
#                    ../Device/src/Hardware/USBD
#                    ../Device/src/Hardware/HAL
#                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFS/src
#                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
#                    ../Device/src/Hardware/USBH
#                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
#                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
#                    ../Device/src/Hardware/FatFS
                    )

add_definitions(-DSTM32F407xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

#add_compile_options(/wd4201)
add_compile_options(/wd4514)
#add_compile_options(/wd4623)
#add_compile_options(/wd4626)
add_compile_options(/wd4820)
#add_compile_options(/wd5027)
#add_compile_options(/wd5039)
add_compile_options(/wd5045)
add_compile_options(/wd5220)    # a non-static data member with a volatile qualified type no longer implies
#add_compile_options(/wd5246)

file(GLOB LIBS_HAL       ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/*.*
                         ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/*.*)

file(GLOB SRC            src/*.h src/*.cpp)
file(GLOB HARDWARE       src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS src/Hardware/CMSIS/*)
file(GLOB HARDWARE_HAL   src/Hardware/HAL/*)

set_source_files_properties(${LIBS_HAL} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(src             FILES ${SRC})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware/CMSIS  FILES ${HARDWARE_CMSIS})
source_group(Hardware/HAL    FILES ${HARDWARE_HAL})

add_library(${PROJECT_NAME} ${LIBS_HAL}
                            ${SRC}
                            ${HARDWARE}
                            ${HARDWARE_CMSIS}
                            ${HARDWARE_HAL}
)
