include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(
    el_test_lib
    ${PROJECT_SOURCE_DIR}/lexer.cpp
    ${PROJECT_SOURCE_DIR}/parser.cpp
    ${PROJECT_SOURCE_DIR}/ast_integer.cpp
    ${PROJECT_SOURCE_DIR}/ast_double.cpp
    ${PROJECT_SOURCE_DIR}/ast_bin_expr.cpp
    ${PROJECT_SOURCE_DIR}/ast_variable.cpp
    ${PROJECT_SOURCE_DIR}/ast_function.cpp

    ${PROJECT_SOURCE_DIR}/../include/ast_bin_expr.h
    ${PROJECT_SOURCE_DIR}/../include/ast_double.h
    ${PROJECT_SOURCE_DIR}/../include/ast_integer.h
    ${PROJECT_SOURCE_DIR}/../include/ast_node.h
    ${PROJECT_SOURCE_DIR}/../include/lexer.h
    ${PROJECT_SOURCE_DIR}/../include/parser.h
    ${PROJECT_SOURCE_DIR}/../include/ast_function.h
    ${PROJECT_SOURCE_DIR}/../include/ast_global_block.h
    ${PROJECT_SOURCE_DIR}/../include/ast_variable.h
    ${PROJECT_SOURCE_DIR}/../include/data_type.h
)

add_executable(
    el_test
    test_parse_expression.cpp
    test_codegen.cpp
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_link_libraries(
    el_test
    gtest_main
    el_test_lib
    ${llvm_libs})

if(UNIX)
    set(CMAKE_CXX_FLAGS "-g -Og -Werror")
endif()

target_include_directories(el_test PUBLIC ${PROJECT_SOURCE_DIR}/../include)
target_include_directories(el_test_lib PUBLIC ${PROJECT_SOURCE_DIR}/../include)
