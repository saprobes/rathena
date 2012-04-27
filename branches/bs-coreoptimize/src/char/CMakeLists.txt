
#
# setup
#
set( SQL_CHAR_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )


#
# char sql
#
if( BUILD_SQL_SERVERS )
message( STATUS "Creating target char-server_sql" )
file( GLOB SQL_CHAR_HEADERS
	"${LOGIN_SOURCE_DIR}/*.h"
	"${LOGIN_SOURCE_DIR}/config/*.h"
	"${LOGIN_SOURCE_DIR}/config/classes/*.h"
	)
file( GLOB SQL_CHAR_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	)
set( DEPENDENCIES common_sql )
set( LIBRARIES ${GLOBAL_LIBRARIES} ${MYSQL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_SQL_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_SQL_DEFINITIONS}" )
set( SOURCE_FILES ${COMMON_SQL_HEADERS} ${SQL_CHAR_HEADERS} ${SQL_CHAR_SOURCES} )
source_group( common FILES ${COMMON_SQL_HEADERS} )
source_group( char FILES ${SQL_CHAR_HEADERS} ${SQL_CHAR_SOURCES} )
include_directories( ${INCLUDE_DIRS} )
add_executable( char-server_sql ${SOURCE_FILES} )
add_dependencies( char-server_sql ${DEPENDENCIES} )
target_link_libraries( char-server_sql ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( char-server_sql PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set_target_properties( char-server_sql PROPERTIES OUTPUT_NAME "char-server_sql-x64" )
endif()
if( INSTALL_COMPONENT_RUNTIME )
	cpack_add_component( Runtime_charserver_sql DESCRIPTION "char-server (sql version)" DISPLAY_NAME "char-server_sql" GROUP Runtime )
	install( TARGETS char-server_sql
		DESTINATION "."
		COMPONENT Runtime_charserver_sql )
endif( INSTALL_COMPONENT_RUNTIME )
message( STATUS "Creating target char-server_sql - done" )
endif( BUILD_SQL_SERVERS )
