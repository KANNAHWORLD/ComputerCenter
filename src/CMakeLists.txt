find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}/_deps/grpc-src/include")
include_directories(${PROJECT_SOURCE_DIR}/src/utils/include)

add_subdirectory(protos)
# Add subdirectories
add_subdirectory(centralServer)
# add_subdirectory(computeNode)
add_subdirectory(utils)

add_subdirectory(client)

add_dependencies(centralServer myproto)
