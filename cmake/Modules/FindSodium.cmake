find_path(
    SODIUM_INCLUDE_DIRS
    NAMES sodium.h
          crypto_box.h
          crypto_stream.h
          crypto_hash.h
    HINTS /usr/local/include
    PATHS /usr/local/include/sodium
          ../libsodium/src/libsodium/include
)

find_library(
    SODIUM_LIBRARIES
    NAMES libsodium.so
    HINTS /usr/local/lib
    PATHS /usr/local/lib/sodium
          ../libsodium/src/libsodium/.libs
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
