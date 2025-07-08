###########################################################
#
# EVS Core Module platform build setup
#
# This file is evaluated as part of the "prepare" stage
# and can be used to set up prerequisites for the build,
# such as generating header files
#
###########################################################

# The list of header files that control the EVS configuration
set(EVS_PLATFORM_CONFIG_FILE_LIST
  cfe_evs_internal_cfg.h
  cfe_evs_msgids.h
  cfe_evs_platform_cfg.h
)

# Create wrappers around the all the config header files
# This makes them individually overridable by the missions, without modifying
# the distribution default copies
foreach(EVS_CFGFILE ${EVS_PLATFORM_CONFIG_FILE_LIST})
  get_filename_component(CFGKEY "${EVS_CFGFILE}" NAME_WE)
  if (DEFINED EVS_CFGFILE_SRC_${CFGKEY})
    set(DEFAULT_SOURCE "${EVS_CFGFILE_SRC_${CFGKEY}}")
  else()
    set(DEFAULT_SOURCE "${CMAKE_CURRENT_LIST_DIR}/config/default_${EVS_CFGFILE}")
  endif()
  generate_config_includefile(
    FILE_NAME           "${EVS_CFGFILE}"
    FALLBACK_FILE       ${DEFAULT_SOURCE}
  )
endforeach()
