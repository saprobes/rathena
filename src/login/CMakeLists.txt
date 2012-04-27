
#
# setup
#
set( LOGIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )
set( SQL_LOGIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )


#
# login sql
#
if( BUILD_SQL_SERVERS )
message( STATUS "Creating target login-server_sql" )
file( GLOB SQL_LOGIN_HEADERS
	"${LOGIN_SOURCE_DIR}/*.h"
	)
file( GLOB SQL_LOGIN_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	)
set( DEPENDENCIES common_sql )
set( LIBRARIES ${GLOBAL_LIBRARIES} ${MYSQL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_SQL_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_SQL_DEFINITIONS}" )
set( SOURCE_FILES ${COMMON_SQL_HEADERS} ${SQL_LOGIN_HEADERS} ${SQL_LOGIN_SOURCES} )
source_group( common FILES ${COMMON_SQL_HEADERS} )
source_group( login FILES ${SQL_LOGIN_HEADERS} ${SQL_LOGIN_SOURCES} )
include_directories( ${INCLUDE_DIRS} )
add_executable( login-server_sql ${SOURCE_FILES} )
add_dependencies( login-server_sql ${DEPENDENCIES} )
target_link_libraries( login-server_sql ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( login-server_sql PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set_target_properties( login-server_sql PROPERTIES OUTPUT_NAME "login-server_sql-x64" )
endif()
if( INSTALL_COMPONENT_RUNTIME )
	cpack_add_component( Runtime_loginserver_sql DESCRIPTION "login-server (sql version)" DISPLAY_NAME "login-server_sql" GROUP Runtime )
	install( TARGETS login-server_sql
		DESTINATION "."
		COMPONENT Runtime_loginserver_sql )
endif( INSTALL_COMPONENT_RUNTIME )
set( TARGET_LIST ${TARGET_LIST} login-server_sql  CACHE INTERNAL "" )
message( STATUS "Creating target login-server_sql - done" )
endif( BUILD_SQL_SERVERS )
