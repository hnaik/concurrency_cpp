cmake_minimum_required(VERSION 3.13)
project(concurrency_cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-fconcepts -Wall -Werror")


add_subdirectory(concurrency)

function(build_program name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} pthread concurrency)
endfunction(build_program)

add_subdirectory(examples)
