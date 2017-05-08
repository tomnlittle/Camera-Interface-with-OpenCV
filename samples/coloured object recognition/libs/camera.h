/*
 * Created on Thu Jan 26 2017 
 *
 * Copyright (c) 2017 Thomas Northall-Little
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define UPDATE_FREQUENCY                                        20.00 

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