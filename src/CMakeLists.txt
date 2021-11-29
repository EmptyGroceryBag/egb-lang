cmake_minimum_required(VERSION 3.0) # arbitrary version requirement
project(el VERSION 0.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable - must be before target_include_directories
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
add_executable(el ${SOURCES})

set(MSVC_STATIC_CRT 1 CACHE BOOL "Windows executable with CRT baked in")
set(GCC_DEBUG 0 CACHE BOOL "Use -G with G++ or clang++")

if(MSVC)
	if(MSVC_STATIC_CRT)
		set(CMAKE_CXX_FLAGS "/EHsc /MT") # statically link CRT
	else()
		set(CMAKE_CXX_FLAGS "/EHsc")
	endif()
else()
	if(GCC_DEBUG)
		set(CMAKE_CXX_FLAGS -g)
	else()
		#set(CMAKE_CXX_FLAGS -O2) # evidently O2 isn't implied with clang. might want to double check with GCC.
	endif()
endif()

# header file to pass project version number to program
# needs to come after set()'s
configure_file(${PROJECT_SOURCE_DIR}/../include/cmake_config.h.in 
	${PROJECT_SOURCE_DIR}/../include/cmake_config.h)
target_include_directories(el PUBLIC ${PROJECT_SOURCE_DIR}/../include)
