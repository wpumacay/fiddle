cmake_minimum_required( VERSION 3.5.0 )
project( felix )

set( CMAKE_BUILD_TYPE Debug )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS_DEBUG "-g -Wno-unused-parameter -std=c++11" )

find_package( CUDA )
find_package( OpenMP )
find_package( Threads REQUIRED )

if( CUDA_FOUND )

add_definitions( -DCUDA_SUPPORT_ENABLED )
include_directories( ${CUDA_INCLUDE_DIRS} )
message( "CUDA SUPPORT ENABLED" )

endif()

if ( OPENMP_FOUND )

set( CMAKE_CXX_FLAGS_DEBUG "-g -Wno-unused-parameter -std=c++11 ${OpenMP_CXX_FLAGS}" )
add_definitions( -DOPENMP_SUPPORT_ENABLED )
message( "OPENMP SUPPORT ENABLED" )

endif()

set( CORE_INCLUDE_DIRS
	 "${CMAKE_SOURCE_DIR}/core/include" )

add_subdirectory( core )
add_subdirectory( tests )
add_subdirectory( viz )

message( "flags: ${CMAKE_CXX_FLAGS_DEBUG}" )