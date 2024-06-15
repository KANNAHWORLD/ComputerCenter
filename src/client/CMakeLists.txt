
set(headers include/client.h
            include/clientCmdl.h
            include/clientTerminalFunctions.h)

set(source lib/client.cpp
            lib/clientCmdl.cpp
            lib/clientTerminalFunctions.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src/protos)
include_directories("${PROJECT_BINARY_DIR}/_deps/grpc-src/include")

add_library(clientlib ${source})
