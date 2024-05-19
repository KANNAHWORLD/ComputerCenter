set(srcs lib/sys_command.cpp)

set(headers "include/spinlock.h"
            "include/sys_command.h")

include_directories(${CMAKE_SOURCE_DIR}/src/protos)

add_library(utils ${srcs})

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)

