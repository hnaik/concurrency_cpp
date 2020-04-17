cmake_minimum_required(VERSION 3.13)
project(concurrency_cpp)

function(build_program name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} pthread)
endfunction(build_program)

add_subdirectory(examples)
