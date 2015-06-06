find_package(PkgConfig)
pkg_check_modules(PC_SODIUM QUIET libsodium)

find_path(
    SODIUM_INCLUDE_DIRS
    NAMES sodium.h
    HINTS ${PC_SODIUM_INCLUDEDIR}
          ${PC_SODIUM_INCLUDE_DIRS}
    PATH_SUFFIXES sodium
)

find_library(
    SODIUM_LIBRARIES
    NAMES libsodium.dylib
    HINTS ${PC_LIBXML_LIBDIR}
          ${PC_LIBXML_LIBRARY_DIRS}
)

#message(WARNING "SODIUM INCLUDE " ${SODIUM_INCLUDE_DIRS})
#message(WARNING "SODIUM LIBRARY " ${SODIUM_LIBRARIES})

if(SODIUM_LIBRARIES AND SODIUM_INCLUDE_DIRS)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SODIUM DEFAULT_MSG SODIUM_LIBRARIES SODIUM_INCLUDE_DIRS)
    mark_as_advanced(SODIUM_LIBRARIES SODIUM_INCLUDE_DIRS)
else()
    message(FATAL_ERROR "Sodium is required, but was not found.")
endif()
