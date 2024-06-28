include(FetchContent)
FetchContent_Declare(
	googletest
	# Specify the commit you depend on and update it regularly.
	URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/central_server/include)
include_directories(${CMAKE_SOURCE_DIR}/src/client/include)
file(GLOB CLIENT_SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/client/lib/clientlib.cpp)
# include_directories(${CMAKE_SOURCE_DIR}/_deps/grpc-src/include)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_util 
				test_utils.cpp)


add_executable(test_server_client 
				test_server_client.cpp)

target_link_libraries(test_util 
						gtest_main 
						utils)

target_link_libraries(test_server_client 
						centralServer
						clientlib
						gtest_main 
						utils
						grpc++
						myproto)

add_test(NAME test_util COMMAND u_gtest)
add_test(NAME test_server_client COMMAND u_gtest)
