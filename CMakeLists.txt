#http://blogs.wcode.org/2014/10/howto-install-build-and-use-opencv-macosx-10-10/

cmake_minimum_required(VERSION 2.8)
project( calibrate )
find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )
MESSAGE( STATUS "OpenCV_INCLUDE_DIRS:         " ${OpenCV_INCLUDE_DIRS} )

add_executable( generate generate.cpp )
target_link_libraries( generate ${OpenCV_LIBS} )