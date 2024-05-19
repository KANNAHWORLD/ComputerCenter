set(srcs lib/sys_command.cpp)

set(headers "include/spinlock.h"
            "include/sys_command.h")

add_library(utils ${srcs})

