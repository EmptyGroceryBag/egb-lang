cmake_minimum_required(VERSION 3.14.5)
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# add the executable - must be before target_include_directories
add_executable(
	el
	main.cpp
	lexer.cpp
)

add_executable(
	hello_test 
	hello_test.cpp
)
target_link_libraries(hello_test gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)

set(MSVC_STATIC_CRT 1 CACHE BOOL "Windows executable with CRT baked in")
set(GCC_DEBUG 0 CACHE BOOL "Tell clang or GCC to produce debug symbols")


if(MSVC)
	if(MSVC_STATIC_CRT)
		set(CMAKE_CXX_FLAGS "/O2 /WX /EHsc /MT") # statically link CRT
	else()
		set(CMAKE_CXX_FLAGS "/O2 /WX /EHsc")
	endif()
else()
	if(GCC_DEBUG)
		set(CMAKE_CXX_FLAGS "-O2 -Werror -g")
	else()
		set(CMAKE_CXX_FLAGS "-O2 -Werror")
	endif()
endif()

# header file to pass project version number to program
# needs to come after set()'s
configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
	${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)
#target_include_directories(el PUBLIC ${GTEST_INCLUDE_DIRS})
