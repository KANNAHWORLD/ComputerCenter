# Getting header files from include directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Protobuf header files
include_directories(${PROTOBUF_INCLUDE_DIRS})

# Utils directory
include_directories("${CMAKE_SOURCE_DIR}/src/utils/include")

# Protocoll buffers that we generated
include_directories("${CMAKE_SOURCE_DIR}/src/protos")

file(GLOB SOURCE_FILES lib/*.cpp)

add_library(centralServer ${SOURCE_FILES})
