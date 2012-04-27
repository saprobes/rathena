
# macro to configure the use of local or system version of a package
# Uses:
#	HAVE_LOCAL_${name} - is local version available?
#	${name}_LOCAL_LIBRARIES - libraries of the local version
#	${name}_LOCAL_INCLUDE_DIRS - include directories of the local version
#	HAVE_SYSTEM_${name} - is system version available?
#	${name}_SYSTEM_LIBRARIES - libraries of the system version
#	${name}_SYSTEM_INCLUDE_DIRS - include directories of the system version
# Generates:
#	WITH_LOCAL_${name} - use the local version of the package (only when local is available)
#	WITH_${name} - use this package
#	${name}_LIBRARIES - libraries
#	${name}_INCLUDE_DIRS - include directories
macro( CONFIGURE_WITH_LOCAL_OR_SYSTEM name )
	unset( ${name}_LIBRARIES CACHE )
	unset( ${name}_INCLUDE_DIRS CACHE )
	if( HAVE_LOCAL_${name} )
		set( WITH_LOCAL_${name} ON
			CACHE BOOL "use local version of ${name}" )
	else()
		unset( WITH_LOCAL_${name} CACHE )
	endif()
	if( WITH_LOCAL_${name} )
		message( STATUS "Configuring for local ${name}" )
		set( ${name}_LIBRARIES ${${name}_LOCAL_LIBRARIES} )
		set( ${name}_INCLUDE_DIRS ${${name}_LOCAL_INCLUDE_DIRS} )
		message( STATUS "Configuring for local ${name} - done" )
	elseif( HAVE_SYSTEM_${name} )
		message( STATUS "Configuring for system ${name}" )
		set( ${name}_LIBRARIES ${${name}_SYSTEM_LIBRARIES} )
		set( ${name}_INCLUDE_DIRS ${${name}_SYSTEM_INCLUDE_DIRS} )
		message( STATUS "Configuring for system ${name} - done" )
	endif()
	if( WITH_LOCAL_${name} OR HAVE_SYSTEM_${name} )
		set( WITH_${name} ON
			CACHE BOOL "use ${name}" )
	else()
		unset( WITH_${name} CACHE )
	endif()
	set( ${name}_LIBRARIES ${${name}_LIBRARIES}
		CACHE PATH "${name} libraries" )
	set( ${name}_INCLUDE_DIRS ${${name}_INCLUDE_DIRS}
		CACHE PATH "${name} include directories" )
	mark_as_advanced( ${name}_LIBRARIES )
	mark_as_advanced( ${name}_INCLUDE_DIRS )
endmacro( CONFIGURE_WITH_LOCAL_OR_SYSTEM )

add_subdirectory( libconfig )
add_subdirectory( msinttypes )
add_subdirectory( mt19937ar )
#add_subdirectory( mysql )
add_subdirectory( mysql-redist )
add_subdirectory( pcre )
#add_subdirectory( zlib )
add_subdirectory( zlib-1.2.6 )


#####################################################################
#
# 3rdparty
#
message( STATUS "Creating target 3rdparty" )
set( 3RDPARTY_HEADERS
	${LIBCONFIG_HEADERS}
	${MT19937AR_HEADERS}
	${ZLIB_HEADERS}
	CACHE INTERNAL "3rdparty headers" )
set( 3RDPARTY_SOURCES
	${LIBCONFIG_SOURCES}
	${MT19937AR_SOURCES}
	${ZLIB_SOURCES}
	CACHE INTERNAL "3rdparty sources" )
set( 3RDPARTY_INCLUDE_DIRS
	${LIBCONFIG_INCLUDE_DIRS}
	${MT19937AR_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIRS}
	CACHE INTERNAL "3rdparty include dirs" )
set( _DEFINITIONS "${LIBCONFIG_DEFINITIONS}" )
if( MSVC )
	set( _DEFINITIONS "${_DEFINITIONS} -DYY_USE_CONST" )
endif()
set( 3RDPARTY_DEFINITIONS
	${_DEFINITIONS}
	CACHE INTERNAL "3rdparty definitions" )
set( LIBRARIES ${GLOBAL_LIBRARIES} )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${3RDPARTY_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${3RDPARTY_DEFINITIONS}" )
set( SOURCE_FILES ${LIBCONFIG_HEADERS} ${LIBCONFIG_SOURCES} ${MT19937AR_HEADERS} ${MT19937AR_SOURCES} ${ZLIB_HEADERS} ${ZLIB_SOURCES} )
source_group( libconfig FILES ${LIBCONFIG_HEADERS} ${LIBCONFIG_SOURCES} )
source_group( mt19937ar FILES ${MT19937AR_HEADERS} ${MT19937AR_SOURCES} )
source_group( zlib FILES ${ZLIB_HEADERS} ${ZLIB_SOURCES} )
add_library( 3rdparty ${SOURCE_FILES} )
target_link_libraries( 3rdparty ${LIBRARIES} )
set_target_properties( 3rdparty PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
include_directories( ${INCLUDE_DIRS} )
set( HAVE_3rdparty ON  CACHE INTERNAL "" )
set( TARGET_LIST ${TARGET_LIST} 3rdparty  CACHE INTERNAL "" )
message( STATUS "Creating target 3rdparty - done" )
