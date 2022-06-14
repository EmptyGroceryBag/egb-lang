add_executable(
  fuzzer
  ${PROJECT_SOURCE_DIR}/../fuzz/fuzz_main.cpp
)

set(CMAKE_CXX_FLAGS "-g -Og -fsanitize=fuzzer")

llvm_map_components_to_libnames(llvm_libs support core irreader fuzzmutate)
target_link_libraries(
  fuzzer 
  el_lib
  ${llvm_libs}
)
target_include_directories(fuzzer PUBLIC ${PROJECT_SOURCE_DIR}/../include)

