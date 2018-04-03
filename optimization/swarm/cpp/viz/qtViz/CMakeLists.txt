
cmake_minimum_required( VERSION 3.5.0 )


message( "Configuring qtViz tool ..." )

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )

include_directories( include/ ui/ ../../core/include )

# MUST set path to qt distribution
set( CMAKE_PREFIX_PATH /home/gregor/Qt/5.10.1/gcc_64 )

# find_package( Qt5Core )
# find_package( Qt5Widgets )
# find_package( Qt5Gui )
# # find_package( Qt5DataVisualization )

find_package( Qt5 COMPONENTS DataVisualization Core Widgets Gui REQUIRED )

add_executable( qtViz 
                main.cpp 
                src/SurfaceHandler.cpp
                mainwindow.cpp )

target_link_libraries( qtViz Qt5::Widgets Qt5::DataVisualization optCore )

message( "Done configuring qtViz" )