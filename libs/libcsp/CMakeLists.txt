# cFS wrapper library for libcsp
project(LIBCSP_WRAPPER C)

# Pull in upstream libcsp (assumes CMakeLists.txt exists in external/libcsp)
# If upstream doesn't provide a CMake, we can add a fallback build later.
add_subdirectory(external/libcsp)

# Define a cFS library target that links to upstream 'csp'
add_cfe_library(libcsp)

# Expose libcsp headers and link against upstream
# Adjust include path if upstream uses a different include layout
# Most libcsp installs expose headers under include/ with <csp/...>
target_include_directories(libcsp PUBLIC external/libcsp/include)
target_link_libraries(libcsp PUBLIC csp)
