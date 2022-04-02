include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    el_test
    test_parse_expression.cpp
    test_codegen.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(
    el_test
    el_lib
    gtest_main
    ${llvm_libs})

if(UNIX)
    set(CMAKE_CXX_FLAGS "-g -Og -Werror")
endif()

target_include_directories(el_test PUBLIC ${PROJECT_SOURCE_DIR}/../include)
