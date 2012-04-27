
#
# Create svnversion.h
#
message( STATUS "Creating svnversion.h" )
if( SVNVERSION )
	file( WRITE ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
		"#ifndef SVNVERSION\n#define SVNVERSION ${SVNVERSION}\n#endif\n" )
else()
	file( WRITE ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h "" )
endif()
set( GLOBAL_INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "" )
set( SVNVERSION ${SVNVERSION}
	CACHE STRING "SVN version of the source code" )
if( INSTALL_COMPONENT_DEVELOPMENT )
	install( FILES ${CMAKE_CURRENT_BINARY_DIR}/svnversion.h
		DESTINATION "src/common"
		COMPONENT Development_base )
endif( INSTALL_COMPONENT_DEVELOPMENT )
message( STATUS "Creating svnversion.h - done" )


#####################################################################
# setup
#
set( COMMON_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
	CACHE PATH "common source directory" )
mark_as_advanced( COMMON_SOURCE_DIR )

set( COMMON_MINI_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/svnversion.h"
	"${COMMON_SOURCE_DIR}/cbasetypes.h"
	"${COMMON_SOURCE_DIR}/core.h"
	"${COMMON_SOURCE_DIR}/mmo.h"
	"${COMMON_SOURCE_DIR}/malloc.h"
	"${COMMON_SOURCE_DIR}/plugin.h"
	"${COMMON_SOURCE_DIR}/showmsg.h"
	"${COMMON_SOURCE_DIR}/strlib.h"
	${LIBCONFIG_HEADERS} # needed by showmsg.h
	CACHE INTERNAL "" )
set( COMMON_MINI_SOURCES
	"${COMMON_SOURCE_DIR}/core.c"
	"${COMMON_SOURCE_DIR}/malloc.c"
	"${COMMON_SOURCE_DIR}/showmsg.c"
	"${COMMON_SOURCE_DIR}/strlib.c"
	${LIBCONFIG_SOURCES} # needed by showmsg.c
	CACHE INTERNAL "" )
set( COMMON_MINI_INCLUDE_DIRS ${LIBCONFIG_INCLUDE_DIRS} CACHE INTERNAL "" )
set( COMMON_MINI_DEFINITIONS "-DMINICORE ${LIBCONFIG_DEFINITIONS}" CACHE INTERNAL "" )


#
# common_sql
#
if( HAVE_3rdparty AND WITH_MYSQL )
message( STATUS "Creating target common_sql" )
file( GLOB _HEADERS "${COMMON_SOURCE_DIR}/*.h" )
file( GLOB _SOURCES "${COMMON_SOURCE_DIR}/*.c" )
set( COMMON_SQL_HEADERS
	"${CMAKE_CURRENT_BINARY_DIR}/svnversion.h"
	${_HEADERS}
	CACHE INTERNAL "common_sql headers" )
set( COMMON_SQL_SOURCES
	${_SOURCES}
	CACHE INTERNAL "common_sql sources" )
set( COMMON_SQL_INCLUDE_DIRS
	${3RDPARTY_INCLUDE_DIRS}
	${MYSQL_INCLUDE_DIRS}
	CACHE INTERNAL "common_sql include dirs" )
set( COMMON_SQL_DEFINITIONS
	${3RDPARTY_DEFINITIONS}
	CACHE INTERNAL "common_sql definitions" )
set( DEPENDENCIES 3rdparty )
set( LIBRARIES ${GLOBAL_LIBRARIES} ${MYSQL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_SQL_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_SQL_DEFINITIONS}" )
set( SOURCE_FILES ${COMMON_SQL_HEADERS} ${COMMON_SQL_SOURCES} )
source_group( common FILES ${COMMON_SQL_HEADERS} ${COMMON_SQL_SOURCES} )
add_library( common_sql ${SOURCE_FILES} )
add_dependencies( common_sql ${DEPENDENCIES} )
target_link_libraries( common_sql ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( common_sql PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
include_directories( ${INCLUDE_DIRS} )
set( HAVE_common_sql ON  CACHE INTERNAL "" )
set( TARGET_LIST ${TARGET_LIST} common_sql  CACHE INTERNAL "" )
message( STATUS "Creating target common_sql - done" )
else()
message( STATUS "Skipping target common_sql (requires 3rdparty and MYSQL)" )
unset( HAVE_common_sql CACHE )
endif()
