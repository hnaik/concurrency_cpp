set(lib_srcs
  src/core/logger.cpp
  src/core/utility.cpp)

add_library(concurrency SHARED ${lib_srcs})

target_include_directories(concurrency PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )
