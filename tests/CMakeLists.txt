include_directories (${mrimrf_SOURCE_DIR}/core) 


link_directories (${mrimrf_BINARY_DIR}/core) 

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

set(test_SOURCES
  test_sw.cc
)

set(test_LIBS
  mrimrf_core
)

add_executable(test_all ${test_SOURCES}
runner.cc
)

target_link_libraries(test_all ${test_LIBS}
  ${Boost_LIBRARIES}
  ${PYTHON_LIBRARY}

 )
add_test(test_all test_all)
