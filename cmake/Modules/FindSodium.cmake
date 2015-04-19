find_package(PkgConfig)
pkg_check_modules(PC_SODIUM QUIET libsodium)

find_path(
    SODIUM_INCLUDE_DIR
    NAMES sodium.h
    HINTS ${PC_SODIUM_INCLUDEDIR}
          ${PC_SODIUM_INCLUDE_DIRS}
    PATH_SUFFIXES sodium
)

find_library(
    SODIUM_LIBRARY
    NAMES libsodium.so
    HINTS ${PC_LIBXML_LIBDIR}
          ${PC_LIBXML_LIBRARY_DIRS}
)

#message(WARNING "SODIUM INCLUDE " ${SODIUM_INCLUDE_DIR})
#message(WARNING "SODIUM LIBRARY " ${SODIUM_LIBRARY})

if(SODIUM_LIBRARY AND SODIUM_INCLUDE_DIR)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(SODIUM DEFAULT_MSG SODIUM_LIBRARY SODIUM_INCLUDE_DIR)
    mark_as_advanced(SODIUM_LIBRARY SODIUM_INCLUDE_DIR)
else()
    message(FATAL_ERROR "Sodium is required, but was not found.")
endif()
