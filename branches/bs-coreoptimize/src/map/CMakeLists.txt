
#
# setup
#
set( MAP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )
set( SQL_MAP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )


#
# map sql
#
if( BUILD_SQL_SERVERS )
message( STATUS "Creating target map-server_sql" )
file( GLOB SQL_MAP_HEADERS
	"${LOGIN_SOURCE_DIR}/*.h"
	)
file( GLOB SQL_MAP_SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
	)
set( DEPENDENCIES common_sql )
set( LIBRARIES ${GLOBAL_LIBRARIES} ${MYSQL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_SQL_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_BASE_DEFINITIONS}" )
if( WITH_PCRE )
	message( STATUS "Enabled PCRE code" )
	set( LIBRARIES ${LIBRARIES} ${PCRE_LIBRARIES} )
	set( INCLUDE_DIRS ${INCLUDE_DIRS} ${PCRE_INCLUDE_DIRS} )
	set( DEFINITIONS "${DEFINITIONS} -DPCRE_SUPPORT" )
else()
	message( STATUS "Disabled PCRE code" )
endif()
set( SOURCE_FILES ${COMMON_SQL_HEADERS} ${SQL_MAP_HEADERS} ${SQL_MAP_SOURCES} )
source_group( common FILES ${COMMON_SQL_HEADERS} )
source_group( map FILES ${SQL_MAP_HEADERS} ${SQL_MAP_SOURCES} )
include_directories( ${INCLUDE_DIRS} )
add_executable( map-server_sql ${SOURCE_FILES} )
add_dependencies( map-server_sql ${DEPENDENCIES} )
target_link_libraries( map-server_sql ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( map-server_sql PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set_target_properties( map-server_sql PROPERTIES OUTPUT_NAME "map-server_sql-x64" )
endif()
if( INSTALL_COMPONENT_RUNTIME )
	cpack_add_component( Runtime_mapserver_sql DESCRIPTION "map-server (sql version)" DISPLAY_NAME "map-server_sql" GROUP Runtime )
	install( TARGETS map-server_sql
		DESTINATION "."
		COMPONENT Runtime_mapserver_sql )
endif( INSTALL_COMPONENT_RUNTIME )
set( TARGET_LIST ${TARGET_LIST} map-server_sql  CACHE INTERNAL "" )
message( STATUS "Creating target map-server_sql - done" )
endif( BUILD_SQL_SERVERS )
