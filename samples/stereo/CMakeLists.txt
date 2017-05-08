cmake_minimum_required(VERSION 2.8)
set (OpenCV_DIR /usr/local/opt/opencv3/share/OpenCV)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
project( main )
find_package( OpenCV REQUIRED )
find_package( Threads )
add_executable( main main.cpp libs/camera.cpp libs/camera.h)

target_link_libraries( main ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} )




