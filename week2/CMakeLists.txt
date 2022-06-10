cmake_minimum_required(VERSION 2.8)

project( denoise )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( denoise denoise.cpp )
target_link_libraries( denoise ${OpenCV_LIBS} )

project( edgeDetect )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( edgeDetect edgeDetect.cpp )
target_link_libraries( edgeDetect ${OpenCV_LIBS} )

project( canny )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( canny canny.cpp )
target_link_libraries( canny ${OpenCV_LIBS} )
