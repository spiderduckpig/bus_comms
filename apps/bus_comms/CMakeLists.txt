project(CFE_BUS_COMMS_APP C)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build using shared libraries" FORCE) 
add_subdirectory(fsw/libcsp) 
set_target_properties(csp PROPERTIES POSITION_INDEPENDENT_CODE ON)

# Create the app module
add_cfe_app(bus_comms fsw/src/bus_comms_app.c)

# Include directories
target_include_directories(bus_comms PUBLIC
  fsw/mission_inc
  fsw/platform_inc
  fsw/libcsp/include
)

# Link the CSP library
target_link_libraries(bus_comms csp)

# Suppress pedantic warnings for the bus_comms app as well
target_compile_options(bus_comms PRIVATE -Wno-error=pedantic)

# Suppress pedantic warnings for libcsp, which uses non-standard C features
target_compile_options(csp PRIVATE 
    -Wno-error=pedantic
    -include ${CMAKE_CURRENT_SOURCE_DIR}/fsw/libcsp/libcsp_cfs_compat.h
)
if(TARGET driver_can)
  target_compile_options(driver_can PRIVATE 
    -Wno-error=pedantic
    -include ${CMAKE_CURRENT_SOURCE_DIR}/fsw/libcsp/libcsp_cfs_compat.h
  )
endif()
if(TARGET if_zmq)
  target_compile_options(if_zmq PRIVATE 
    -Wno-error=pedantic
    -include ${CMAKE_CURRENT_SOURCE_DIR}/fsw/libcsp/libcsp_cfs_compat.h
  )
endif()

# If UT is enabled, add tests in subdirectory (optional placeholder)
if (ENABLE_UNIT_TESTS)
  # add_subdirectory(unit-test)
endif ()

