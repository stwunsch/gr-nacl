find_path(
    NACLLIB_INCLUDE_DIRS
    NAMES crypto_box.h
          crypto_stream.h
          crypto_hash.h
    HINTS /usr/local/lib
    PATHS /usr/local/lib/nacl/include/amd64
)

find_library(
    NACLLIB_LIBRARIES
    NAMES libnacl.a
    HINTS /usr/local/lib
    PATHS /usr/local/lib/nacl/lib/amd64
)

#message(STATUS "INCLUDE " ${NACLLIB_INCLUDE_DIRS})
#message(STATUS "LIBRARY " ${NACLLIB_LIBRARIES})

if(NACLLIB_LIBRARIES AND NACLLIB_INCLUDE_DIRS)
    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(NACLLIB DEFAULT_MSG NACLLIB_LIBRARIES NACLLIB_INCLUDE_DIRS)
    mark_as_advanced(NACLLIB_LIBRARIES NACLLIB_INCLUDE_DIRS)
else()
    message(FATAL_ERROR "NaCl is required, but was not found.")
endif()
