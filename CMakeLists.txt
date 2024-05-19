cmake_minimum_required(VERSION 3.8)

# Project name
project(ComputeCenter)

# Build for C++20 if we can
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BENCHMARK_USE_BUNDLED_GTEST OFF)


# Fetch and configure protobuf
include(FetchContent)


set(ABSL_ENABLE_INSTALL ON)
# include(FetchContent)
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc
  GIT_TAG        v1.63.0
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

# include(FetchContent)
add_compile_options(-stdlib=libc++)

# Turn on any compiler-specific flags
if (WIN32)
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
else()
	# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
	# if ("$ENV{SANITIZE}" STREQUAL "ON")
	# 	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
	# endif()
	
	if(APPLE)
	# 	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Debug] "YES")
	# 	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=MinSizeRel] "NO")
	# 	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=RelWithDebInfo] "YES")
	# 	set(CMAKE_XCODE_ATTRIBUTE_GCC_GENERATE_DEBUGGING_SYMBOLS[variant=Release] "NO")
	endif()
endif()

# Specify the build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Any source files in this directory
set(SOURCE_FILES main.cpp)

set(BENCHMARK_SOURCE_FILES benchmarks.cpp)

# Subdirectories to build
# add_subdirectory(src)
add_subdirectory(benchmark)

add_subdirectory(src)

include_directories(${CMAKE_SOURCE_DIR}/src/utils/include)

# Name of executable
add_executable(main ${SOURCE_FILES})


# Name of executable
add_executable(mainBenchmark ${BENCHMARK_SOURCE_FILES})

# Link main vs the source library
target_link_libraries(main clientlib 
							centralServer 
							grpc++ 
							myproto 
							utils)

# Link mainBenchmark vs the source library
target_link_libraries(mainBenchmark benchmark::benchmark)
