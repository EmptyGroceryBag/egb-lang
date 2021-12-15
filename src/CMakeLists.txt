cmake_minimum_required(VERSION 3.14.5)
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BUILD_TEST 0 CACHE BOOL "Build test executable")
if(BUILD_TEST)
	add_library(
		el
		lexer.cpp
		main.cpp
		parser.cpp
		ast_integer.cpp
		ast_double.cpp

		${PROJECT_SOURCE_DIR}/../include/lexer.h
		${PROJECT_SOURCE_DIR}/../include/parser.h
		${PROJECT_SOURCE_DIR}/../include/ast_node.h
		${PROJECT_SOURCE_DIR}/../include/ast_integer.h
		${PROJECT_SOURCE_DIR}/../include/ast_double.h
	)

	add_subdirectory(${PROJECT_SOURCE_DIR}/../test test)
else()
	add_executable(
		el
		lexer.cpp
		main.cpp
		parser.cpp
		ast_integer.cpp
		ast_double.cpp

		${PROJECT_SOURCE_DIR}/../include/lexer.h
		${PROJECT_SOURCE_DIR}/../include/parser.h
		${PROJECT_SOURCE_DIR}/../include/ast_node.h
		${PROJECT_SOURCE_DIR}/../include/ast_integer.h
		${PROJECT_SOURCE_DIR}/../include/ast_double.h
	)
endif()

#include(GoogleTest)
#gtest_discover_tests(hello_test)

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
		set(CMAKE_CXX_FLAGS "-Werror -g")
	else()
		set(CMAKE_CXX_FLAGS "-O2 -Werror")
	endif()
endif()

# needs to come after set()'s
configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
	${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)
