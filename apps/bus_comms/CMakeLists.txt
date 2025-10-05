project(CFE_BUS_COMMS_APP C)

# Create the app module
add_cfe_app(bus_comms fsw/src/bus_comms_app.c)

# Include directories
target_include_directories(bus_comms PUBLIC
  fsw/mission_inc
  fsw/platform_inc
)

# If UT is enabled, add tests in subdirectory (optional placeholder)
if (ENABLE_UNIT_TESTS)
  # add_subdirectory(unit-test)
endif ()

# Link bus_comms against mission libcsp library
add_cfe_app_dependency(bus_comms libcsp)
