# The OpenCV Camera Library 

**Simple open-source library for effortlessly grabbing frames from a sensor.**

Once Initialised with the correct attributes (or none at all if you like) the library will:

- Use Multi-threading to avoid blocking operations on your program, and ensure that a frame is always accessible 
- Noise Reduction - on new frames before they are passed back 
- Frame flipping - Ne frames can be flipped in their orientation, useful for mirror effects

##How To

Simply Cmake the library as you would any normal OpenCV project, be sure to include into your add executable. 

```

    libs/camera.h libs/camera.cpp

```

In the above example this is the cmake file 

```

    cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
    project(CameraInterface)
    set (OpenCV_DIR /usr/local/opt/opencv3/share/OpenCV)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
    find_package( OpenCV REQUIRED )
    find_package( Threads )

    add_executable(run example.cpp libs/camera.h libs/camera.cpp )

    target_link_libraries(run ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

```




##TODO
- Increase the effeciency on noise reduction as it can be pretty unsuable on fluid videos
- Add Call and Run support - essentially be able to just call the library and have it work out which port the camera is on

