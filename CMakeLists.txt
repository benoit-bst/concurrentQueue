cmake_minimum_required(VERSION 2.8)
project(ConcurrentQueue)

set(COVERAGE OFF CACHE BOOL "Coverage")

include(gtest.cmake)
find_package(Threads)

SET(Sources
  units_tests.cpp
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -Wall -Wextra")

add_executable(units_tests ${Sources})
target_link_libraries(units_tests PRIVATE gtest ${CMAKE_THREAD_LIBS_INIT})

if (COVERAGE)
  target_compile_options(tests PRIVATE --coverage)
  target_link_libraries(tests PRIVATE --coverage)
endif()
