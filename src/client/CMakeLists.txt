
set(headers include/mainClient.h
            "include/clientRemoteTerminal.h"
            "include/clientLocalTerminal.h"
            )

set(source lib/mainClient.cpp
            lib/clientRemoteTerminal.cpp
            lib/clientLocalTerminal.cpp
            )


# target_link_libraries(clientlib PRIVATE myproto)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/protos)
include_directories(${CMAKE_SOURCE_DIR}/_deps/grpc-src/include)

add_library(clientlib ${source})
