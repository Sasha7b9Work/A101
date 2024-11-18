# (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (hex2binA101)

#set(CMAKE_UNITY_BUILD OFF)

include_directories(
    .
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MTd")


add_compile_options(
#    /wd4355    # 'this': used in base member initializer list
#    /wd4514    # 'xxx': unreferenced inline function has been removed
#    /wd4625    # copy constructor was implicitly defined as deleted
#    /wd4626    # assignment operator was implicitly defined as deleted
#    /wd4820    # 'bytes' bytes padding added after construct 'member_name'
#    /wd5026    # move constructor was implicitly defined as deleted
#    /wd5027    # move assignment operator was implicitly defined as deleted
#    /wd5039    # pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc
#    /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#    /wd5219    # implicit conversion from 'int' to 'float', possible loss of data
#    /wd5264    # 'const' variable is not used
#    /wd5267
)


file(GLOB SRC       *.cpp *.h)

source_group(src    FILES ${SRC})

add_executable(
    ${PROJECT_NAME}
    ${SRC}
)


target_link_libraries(${PROJECT_NAME} Ws2_32)

target_link_libraries(${PROJECT_NAME} WinMM.lib)
target_link_libraries(${PROJECT_NAME} version.lib)
target_link_libraries(${PROJECT_NAME} Imm32.Lib)
target_link_libraries(${PROJECT_NAME} Setupapi.lib)
