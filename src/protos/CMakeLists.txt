find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

set(GRPC_CPP_PLUGIN "${PROJECT_BINARY_DIR}/_deps/grpc-build/grpc_cpp_plugin")
# include_directories("/opt/homebrew/include")/

set(PROTO_SOURCE_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/command.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/command.grpc.pb.cc
)

set(PROTO_HEADER_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/command.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/command.grpc.pb.h
)

# set(PROTOBUF_PROTOC_EXECUTABLE /opt/homebrew/bin/protoc)

message(STATUS "Protobuf found: ${PROTOBUF_FOUND}")
message(STATUS "Protobuf include directories: ${PROTOBUF_INCLUDE_DIRS}")
message(STATUS "Protobuf libraries: ${PROTOBUF_LIBRARIES}")

add_custom_command(
    OUTPUT 
        ${PROTO_SOURCE_FILES} ${PROTO_HEADER_FILES}
    COMMAND 
        ${PROTOBUF_PROTOC_EXECUTABLE} 
        --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
        --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
        --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
        -I${CMAKE_CURRENT_SOURCE_DIR} 
        ${CMAKE_CURRENT_SOURCE_DIR}/command.proto
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/command.proto
    COMMENT "Running C++ protocol buffer compiler on command.proto"
)

add_library(myproto ${PROTO_SOURCE_FILES})

# target_link_libraries(myproto ${PROTOBUF_LIBRARIES})
