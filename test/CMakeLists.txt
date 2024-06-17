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

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(test_util test_utils.cpp)
target_link_libraries(test_util gtest_main utils)
add_test(NAME test_util COMMAND u_gtest)
