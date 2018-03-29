
message( "CONFIGURING CORE" )

include_directories( ${CORE_INCLUDE_DIRS} )

FILE( GLOB_RECURSE CoreSources src *.cpp )

add_library( optCore
             ${CoreSources} )

target_link_libraries( optCore
                       ${CMAKE_THREAD_LIBS_INIT} )