OPTION( BUILD_REPORTING "Build Reporting Component" ON )
# Set to OFF by default as it does not compile
OPTION( BUILD_REPORTING_NETCDF "Build the Netcdf Reporting Component" OFF )

IF ( BUILD_REPORTING )

    find_package(RTTPlugin REQUIRED rtt-marshalling)

    # This gathers all the .cpp files into the variable 'SRCS'
    SET( SRCS ConsoleReporting.cpp FileReporting.cpp ReportingComponent.cpp )
    SET( HPPS ConsoleReporting.hpp  FileReporting.hpp NiceHeaderMarshaller.hpp ReportingComponent.hpp)

    # Reporting to a socket
    SET( SOCKET_SRCS command.cpp datasender.cpp socket.cpp socketmarshaller.cpp TcpReporting.cpp)
    SET( SOCKET_HPPS command.hpp datasender.hpp socket.hpp socketmarshaller.hpp TcpReporting.hpp)

    if(NOT OROCOS_TARGET STREQUAL "win32")
      set(SRCS ${SRCS} ${SOCKET_SRCS})
      set(HPPS ${HPPS} ${SOCKET_HPPS})
    endif()

    INCLUDE_DIRECTORIES ( ${BOOST} ${BOOST_OPTIONS} )

    GLOBAL_ADD_COMPONENT( orocos-reporting ${SRCS})
    GLOBAL_ADD_INCLUDE( ocl ${HPPS})
    COMPONENT_ADD_DEPS( orocos-reporting )

    IF ( BUILD_REPORTING_NETCDF AND NETCDF_FOUND )
      SET( NETCDF_SRCS NetcdfReporting.cpp )
      SET( NETCDF_HPPS NetcdfReporting.hpp NetcdfMarshaller.hpp NetcdfHeaderMarshaller.hpp )

      GLOBAL_ADD_COMPONENT( orocos-reporting-netcdf ${NETCDF_SRCS})
      GLOBAL_ADD_INCLUDE ( ocl ${NETCDF_HPPS} )
      COMPONENT_ADD_LIBS( orocos-reporting-netcdf ${NETCDF_LIBS} )
      COMPONENT_ADD_DEPS( orocos-reporting-netcdf orocos-reporting )
    ENDIF ( BUILD_REPORTING_NETCDF AND NETCDF_FOUND )
    
    COMPONENT_ADD_LIBS( orocos-reporting ${RTT_PLUGIN_rtt-marshalling_LIBRARIES} )
    
    ADD_SUBDIRECTORY( tests )
ENDIF ( BUILD_REPORTING )
