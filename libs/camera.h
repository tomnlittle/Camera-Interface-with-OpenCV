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

class Camera {
    public:
        Camera( int camID = 0, float noise = 0, bool mirror = false, cv::Size photoSize = cv::Size(0,0));
        ~Camera();

        cv::Mat3b getFrame();   //returns the last completed frame

        /*
            Always returns a new frame, whereas getFrame will always load the current frame this function will block
            until a new frame can be returned.
        */
        cv::Mat3b getNewFrame(); 

        /*
            Will return the average of a number of frames, useful for noise reduction
        */

        cv::Mat3b getAveragedFrame(int);

    private:
        void update();

        std::thread updateThread; 

        cv::VideoCapture camera;  
        cv::Mat3b frame;
        cv::Size size;

        int cameraNumber;
        int updateCount;
        int frameNum; //Used to keep track of the frame that was last returned
        bool threadActive;
        bool shouldFlip;
        float noise_reduction_level;
};