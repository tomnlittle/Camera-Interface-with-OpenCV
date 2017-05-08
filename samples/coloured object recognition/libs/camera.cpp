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

Camera::Camera(int num, int noise, cv::Size photoSize){
    cameraNumber = num;
    camera = cv::VideoCapture(cameraNumber);
    size = photoSize;

    camera >> frame;
    threadActive = true;
    updateCount = 0;
    noise_reduction_level = noise;
   // if(!camera.isOpened()) return 1;
   updateThread = std::thread(&Camera::update, this);
}

Camera::~Camera(){
    stop();
}

void Camera::update(){
    cv::Mat3b in_frame;
    cv::Mat3b resized_frame;
    cv::Mat3b denoised_frame;
    cv::Size emptySize(0,0);

    while(threadActive){
        camera >> in_frame;

        if(size != emptySize){
            cv::resize(in_frame, resized_frame, size, 0, 0, cv::INTER_CUBIC);
            in_frame = resized_frame;
        } 

        if(noise_reduction_level != 0){
            cv::fastNlMeansDenoising(in_frame, denoised_frame, noise_reduction_level, 11, 5);
            in_frame = denoised_frame;
        } 
        
        usleep(UPDATE_FREQUENCY*100.00);
        frame = in_frame;
        updateCount++;
    }
}

void Camera::stop(){
    threadActive = false;
    if(updateThread.joinable()){
        updateThread.join();
    }
    std::cout << "Camera " << cameraNumber << " Halted: Number of Updates " << updateCount << "\n";
}

cv::Mat3b Camera::getFrame(){
    return frame;
}

int Camera::getCount(){
    return updateCount;
}