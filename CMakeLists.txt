#
# Test CMake version
#
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
#MARK_AS_ADVANCED( FORCE CMAKE_BACKWARDS_COMPATIBILITY )

# for CMake 2.6 corrected behaviour (see "cmake --help-policy CMP0003")
IF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 4)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} GREATER 4)


###################################################
#                                                 #
#       Start project customization section       #
#                                                 #
###################################################

PROJECT(orocos-ocl)

SET( OCL_VERSION 1.99.5 )
STRING( REGEX MATCHALL "[0-9]+" OCL_VERSIONS ${OCL_VERSION} )
LIST( GET OCL_VERSIONS 0 OCL_VERSION_MAJOR)
LIST( GET OCL_VERSIONS 1 OCL_VERSION_MINOR)
LIST( GET OCL_VERSIONS 2 OCL_VERSION_PATCH)
MESSAGE( "Orocos Component Library version ${VERSION} (${OCL_VERSION_MAJOR}.${OCL_VERSION_MINOR}.${OCL_VERSION_PATCH})" )

SET( PROJ_SOURCE_DIR ${orocos-ocl_SOURCE_DIR} )
SET( PROJ_BINARY_DIR ${orocos-ocl_BINARY_DIR} )

# Load the optional orocos-ocl.cmake file.
INCLUDE(${PROJ_SOURCE_DIR}/orocos-ocl.cmake OPTIONAL)
if (NOT EXISTS ${PROJ_SOURCE_DIR}/orocos-ocl.cmake)
  # Setup the default settings
  INCLUDE(${PROJ_SOURCE_DIR}/orocos-ocl.default.cmake)
  MESSAGE(
    "No orocos-ocl.cmake file loaded, using default settings."
    "See orocos-ocl.default.cmake")
endif ()

INCLUDE(CTest)
ENABLE_TESTING()
IF(BUILD_TESTING)
  SET(BUILDNAME "${BUILDNAME}" CACHE STRING "Name of build on the dashboard")
  MARK_AS_ADVANCED(BUILDNAME)
ENDIF(BUILD_TESTING)

# Do not rely on environment variables to find the path, because that's confusing
IF(NOT CMAKE_INSTALL_PREFIX)
  SET( CMAKE_INSTALL_PREFIX /usr/local CACHE PATH "Installation directory" FORCE)
  MESSAGE( "Setting installation directory to ${CMAKE_INSTALL_PREFIX}" )
ENDIF(NOT CMAKE_INSTALL_PREFIX)

# Used by Doxygen.in !
IF(NOT RTT_VVERSION)
  SET( RTT_VVERSION "v${OCL_VERSION_MAJOR}.${OCL_VERSION_MINOR}.x" )
ENDIF(NOT RTT_VVERSION)

IF(NOT NETCDF_INSTALL)
  SET( NETCDF_INSTALL ${CMAKE_INSTALL_PREFIX} CACHE PATH "The NETCDF installation directory.")
ENDIF(NOT NETCDF_INSTALL)

# The dependencies of the COMPONENTS_LIBRARY
SET( COMPONENTS_LIBRARY_DEPS "" CACHE STRING "Dependencies of the resulting library")

# Make compilation verbose by default
SET(CMAKE_VERBOSE_MAKEFILE FALSE)

###################################################
#                                                 #
#       End project customization section         #
#                                                 #
###################################################

if(OROCOS_TARGET STREQUAL "win32")
  if (MSVC)
    if (NOT MSVC80)
      set(NUM_PARALLEL_BUILD 4 CACHE STRING "Number of parallel builds")
      set(PARALLEL_FLAG "/MP${NUM_PARALLEL_BUILD}")
    endif()
    add_definitions(/wd4355 /wd4251 /wd4180 /wd4996 /bigobj ${PARALLEL_FLAG} /DBOOST_ALL_NO_LIB)
    #list(APPEND OROCOS-RTT_LIBRARIES kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib  ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib Ws2_32.lib winmm.lib)
  endif()
  set(CMAKE_DEBUG_POSTFIX "d")
endif(OROCOS_TARGET STREQUAL "win32")

###################################################
#                                                 #
#       Do some global set-up & checking          #
#                                                 #
###################################################

# Modules path (for searching FindXXX.cmake files)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/config")

#
# This is the first in the include path.
INCLUDE_DIRECTORIES( ${PROJ_SOURCE_DIR} ${PROJ_BINARY_DIR}/ocl )

# Put your 'flags' inhere if you want them to apear in the
# orocos-components.pc file. Various .cmake scripts below do
# this.
SET( ENV{OROCOS_COMPONENTS_CFLAGS} "")
SET( ENV{OROCOS_COMPONENTS_LINKFLAGS} "")
SET( ENV{OROCOS_COMPONENTS_REQUIRES} "")

INCLUDE(${PROJ_SOURCE_DIR}/config/component_rules.cmake)
INCLUDE(${PROJ_SOURCE_DIR}/config/TestUsesFile.cmake)

## Find Orocos RTT.
find_package(OROCOS-RTT 1.99.5)
if (NOT OROCOS-RTT_FOUND)
  message(FATAL_ERROR "\n   RTT not found. Is the version correct? Use the CMAKE_PREFIX_PATH cmake or environment variable to point to the installation directory of RTT.")
else()
  include(${OROCOS-RTT_USE_FILE_PATH}/UseOROCOS-RTT.cmake)
  add_definitions( -DRTT_COMPONENT )
endif()

## Find Netcdf
INCLUDE (${PROJ_SOURCE_DIR}/config/FindNetcdf.cmake)

## Uninstall target
CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

OPTION( NO_GPL "Set to 'ON' to exclude all GPL'd code" OFF)
IF(NO_GPL)
  ADD_DEFINITIONS( -DNO_GPL )
ENDIF(NO_GPL)


###########################################################
#                                                         #
# Look for dependencies required by individual components #
#                                                         #
###########################################################

INCLUDE(${PROJ_SOURCE_DIR}/config/check_depend.cmake)

#First unset the LOCAL_LIBRARIES
SET(ENV{SELECTED_LIBS} "")
SET(ENV{SELECTED_DIRS} "")
ADD_SUBDIRECTORY( doc )
ADD_SUBDIRECTORY( ocl )
ADD_SUBDIRECTORY( taskbrowser )
ADD_SUBDIRECTORY( timer )
ADD_SUBDIRECTORY( helloworld )
ADD_SUBDIRECTORY( reporting )
ADD_SUBDIRECTORY( deployment )
ADD_SUBDIRECTORY( bin )
ADD_SUBDIRECTORY( logging )

MESSAGE("Selected Libraries: $ENV{SELECTED_LIBS}")
SET(SELECTED_LIBS $ENV{SELECTED_LIBS})
SET(OROCOS_COMPONENTS_LINKFLAGS "$ENV{OROCOS_COMPONENTS_LINKFLAGS}")
SET(OROCOS_COMPONENTS_CFLAGS "$ENV{OROCOS_COMPONENTS_CFLAGS}")
SET(OROCOS_COMPONENTS_REQUIRES "$ENV{OROCOS_COMPONENTS_REQUIRES}")

CONFIGURE_FILE( ${PROJ_SOURCE_DIR}/ocl/ocl-config.h.in ${PROJ_BINARY_DIR}/ocl/ocl-config.h @ONLY)
CONFIGURE_FILE( ${PROJ_SOURCE_DIR}/orocos-ocl.pc.in ${PROJ_BINARY_DIR}/orocos-ocl-${OROCOS_TARGET}.pc @ONLY)
INSTALL_FILES( /lib/pkgconfig FILES orocos-ocl-${OROCOS_TARGET}.pc)
INSTALL_FILES( /include/ocl FILES ocl/ocl-config.h)

#MESSAGE("Selected dirs: $ENV{SELECTED_DIRS}")
#SET(SELECTED_DIRS $ENV{SELECTED_DIRS})

CONFIGURE_FILE(Doxyfile.in Doxyfile @ONLY)

#############################
#			    #
# Building Doxygen documents#
#                           #
#############################

ADD_CUSTOM_TARGET(docapi "doxygen" "Doxyfile")

## Debugging
OPTION( DISPLAY_FLAGS "Display cmake variables at end of configure step." OFF)
MARK_AS_ADVANCED(DISPLAY_FLAGS)
IF( DISPLAY_FLAGS )
INCLUDE(config/cmake_logging.cmake)
ENDIF( DISPLAY_FLAGS )
