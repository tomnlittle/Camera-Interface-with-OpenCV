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
 
#include "camera.h"

Camera::Camera(int num, float noise, bool flip, cv::Size photoSize){
    cameraNumber = num;
    camera = cv::VideoCapture(cameraNumber);
    size = photoSize;
    threadActive = true;
    updateCount = 0;
    frameNum = 0;
    noise_reduction_level = noise;
    shouldFlip = flip;
    updateThread = std::thread(&Camera::update, this);
}

Camera::~Camera(){
    threadActive = false;
    if(updateThread.joinable()){
        updateThread.join();
    }
    std::cout << "Camera " << cameraNumber << " Halted: Number of Updates " << updateCount << "\n";
}

void Camera::update(){
    cv::Size emptySize(0,0);
    while(threadActive){
        cv::Mat3b in_frame;
        camera >> in_frame;

        if(size != emptySize){
            cv::Mat3b resized_frame;
            cv::resize(in_frame, resized_frame, size, 0, 0, cv::INTER_CUBIC);
            resized_frame.copyTo(in_frame);
        } 

        if(noise_reduction_level > 0){
            cv::Mat bgr[3];   
            cv::split(in_frame, bgr);

            std::vector<cv::Mat> array_of_Mats;

            cv::fastNlMeansDenoising(bgr[0], bgr[0], noise_reduction_level);
            cv::fastNlMeansDenoising(bgr[1], bgr[1], noise_reduction_level);
            cv::fastNlMeansDenoising(bgr[2], bgr[2], noise_reduction_level);

            array_of_Mats.push_back(bgr[0]);
            array_of_Mats.push_back(bgr[1]);
            array_of_Mats.push_back(bgr[2]);

            cv::Mat denoised_frame;
            cv::merge(array_of_Mats, denoised_frame);

            denoised_frame.copyTo(in_frame);
        } 

        if(shouldFlip){
            cv::Mat3b flipped;
            cv::flip(in_frame, flipped, 1);
            flipped.copyTo(in_frame);
        }

        in_frame.copyTo(frame);
        updateCount++;
    }
}

cv::Mat3b Camera::getFrame(){
    while(frame.empty()); //blocking operation while the first frame comes through
    frameNum = updateCount;
    return frame;
}

cv::Mat3b Camera::getNewFrame(){
    while(frame.empty()); //blocking operation while the first frame comes through
    while(frameNum == updateCount); //blocking operation which makes the system wait for new frame
    frameNum = updateCount;
    return frame;
}
