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
 

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include "../../libs/camera.h"

bool run;
void command();

int main(int argc, char** argv )
{
    Camera cam0(0); // Camera 1
    Camera cam1(1); // Camera 2

    run = true;     // initialises the run boolean
    std::thread commandsThread(command);    
   
    while(run){
        cv::Mat f1, f2;
        f1 = cam0.getNewFrame();     
        f2 = cam1.getNewFrame();
        imshow("Cam1", f1);
        imshow("Cam2", f2);
        cv::waitKey(1); 
    }

    commandsThread.join();
    return EXIT_SUCCESS;
}

void command(void){
    std::string hold_value = " ";
    printf("Press any key to close\n");
    std::cin >> hold_value;	
    run = false; // causes program to finish
    return;
}
