###########################################################
#
# TIME Core Module mission build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the TIME configuration
set(TIME_MISSION_CONFIG_FILE_LIST
  cfe_time_mission_cfg.h
  cfe_time_interface_cfg.h
  cfe_time_extern_typedefs.h
  cfe_time_fcncodes.h
  cfe_time_msgdefs.h
  cfe_time_msg.h
  cfe_time_msgstruct.h
  cfe_time_topicids.h
)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(TIME_CFGFILE ${TIME_MISSION_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${TIME_CFGFILE}" NAME_WE)
  if (DEFINED TIME_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE GENERATED_FILE "${TIME_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE FALLBACK_FILE "${CMAKE_CURRENT_LIST_DIR}/config/default_${TIME_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${TIME_CFGFILE}"
    ${DEFAULT_SOURCE}
  )
endforeach()
