
include_directories (${mrimrf_SOURCE_DIR}/core) 


link_directories (${mrimrf_BINARY_DIR}/core) 

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

set(pywrap_SOURCES
pywrap.cc
)
