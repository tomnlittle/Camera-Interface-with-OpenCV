#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include "opencv2/gpu/gpu.hpp"
#include <thread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define UPDATE_FREQUENCY                                        50.00 

class Camera {
    public:
        Camera(int cameraNumber, int noise, cv::Size);
        ~Camera();
        cv::Mat3b getFrame();
    private:
        void update();
        void stop();
        int getCount();

        cv::Mat3b frame;
        int cameraNumber;
        bool threadActive;
        std::thread updateThread;  
        cv::VideoCapture camera;  
        int updateCount;
        int noise_reduction_level;
        cv::Size size;
};