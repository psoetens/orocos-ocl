OPTION( BUILD_LOGGING "Build realtime Logging service" OFF)

IF ( BUILD_LOGGING )
  if(NOT LOG4CPP_FOUND)
    message(SEND_ERROR "Can't build real-time logging service since log4cpp was not found.")
  endif()

  FILE( GLOB CPPS [^.]*.cpp )
  FILE( GLOB HPPS [^.]*.hpp )

  INCLUDE_DIRECTORIES( "${LOG4CPP_INCLUDE_DIRS}" )
  LINK_DIRECTORIES( "${LOG4CPP_LIBRARY_DIRS}" )

  GLOBAL_ADD_COMPONENT( orocos-logging ${CPPS} )
  GLOBAL_ADD_INCLUDE( ocl ${HPPS})
  COMPONENT_ADD_LIBS( orocos-logging "${LOG4CPP_LIBRARIES}")

  ADD_SUBDIRECTORY( tests )

ENDIF ( BUILD_LOGGING )

