INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_NACL nacl)

FIND_PATH(
    NACL_INCLUDE_DIRS
    NAMES nacl/api.h
    HINTS $ENV{NACL_DIR}/include
        ${PC_NACL_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    NACL_LIBRARIES
    NAMES gnuradio-nacl
    HINTS $ENV{NACL_DIR}/lib
        ${PC_NACL_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(NACL DEFAULT_MSG NACL_LIBRARIES NACL_INCLUDE_DIRS)
MARK_AS_ADVANCED(NACL_LIBRARIES NACL_INCLUDE_DIRS)
