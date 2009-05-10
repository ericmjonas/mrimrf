
link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

# core library

set(mrimrf_core_SOURCES
test.cc
util.cc
mrimrf.cc
sw.cc
)


add_library(mrimrf_core ${mrimrf_core_SOURCES})
#target_link_libraries(crosscat_core component_models)

include(FindPythonLibs)
include_directories(${PYTHON_INCLUDE_PATH})

add_library(pymrimrf SHARED
pymrimrf.cc

)

target_link_libraries(pymrimrf
mrimrf_core
${Boost_LIBRARIES}
)

set_target_properties(pymrimrf PROPERTIES PREFIX "")

set(irm_python_install_DIR lib/site-python/irm)
