cmake_minimum_required(VERSION 2.6)

project(mrimrf)

#enable_testing()


set(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(Boost REQUIRED unit_test_framework python)

include_directories(core)
include_directories(pywrap)

SET(CMAKE_CXX_FLAGS "-mtune=k8 -msse3 -mfpmath=sse -Wno-deprecated -O3 -fPIC ${CMAKE_CXX_FLAGS}")

add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(pywrap)

