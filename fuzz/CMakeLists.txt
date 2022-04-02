add_executable(
  fuzzer
  ${PROJECT_SOURCE_DIR}/fuzz_main.cpp
)

set(CMAKE_CXX_FLAGS "-g -Og -fsanitize=fuzzer")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "clang")

llvm_map_components_to_libnames(llvm_libs support core irreader fuzzmutate)
target_link_libraries(
  fuzzer 
  el_lib
  ${llvm_libs}
)
target_include_directories(fuzzer PUBLIC ${PROJECT_SOURCE_DIR}/../include)

