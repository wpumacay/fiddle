
message( "CONFIGURING CORE" )

include_directories( ${CORE_INCLUDE_DIRS} )

set( CoreSources
     src/BenchmarkFunctions.cpp
     src/Common.cpp
     src/OptimizerInterface.cpp
     src/OptimizerPSO.cpp
     src/OptimizerPSOcuda.cpp )


if ( CUDA_FOUND )

    include_directories( ${CUDA_INCLUDE_DIRS} )

    set( CoreCudaSources
         src/PSOcudaHelpers.cu )

    cuda_add_library( optCore
                      ${CoreSources}
                      ${CoreCudaSources} )

    target_link_libraries( optCore
                           ${CMAKE_THREAD_LIBS_INIT} )

else()

    add_library( optCore
                 ${CoreSources} )

    target_link_libraries( optCore
                           ${CMAKE_THREAD_LIBS_INIT} )

endif()
