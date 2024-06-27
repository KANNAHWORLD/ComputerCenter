set(srcs lib/sys_command.cpp
        lib/stringOps.cpp)

set(headers "include/spinlock.h"
            "include/sys_command.h"
            "include/stringOps.h"
            "include/typeCheck.h")

include_directories(${CMAKE_SOURCE_DIR}/src/protos)

add_library(utils ${srcs})

target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)

