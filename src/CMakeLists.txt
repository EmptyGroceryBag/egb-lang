cmake_minimum_required(VERSION 3.0) # arbitrary version requirement
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable - must be before target_include_directories
add_executable(el ${PROJECT_SOURCE_DIR}/parser.cpp)

if(MSVC)
	if(MSVC_STATIC_CRT)
		set(CMAKE_CXX_FLAGS /MT) # statically link CRT
	else()
		set(MSVC_STATIC_CRT 0)
	endif()
else()
	set(CMAKE_CXX_FLAGS -O2) # evidently O2 isn't implied with clang. might want to double check with GCC.
endif()

# header file to pass project version number to program
# needs to come after set()'s
configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
	${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)
