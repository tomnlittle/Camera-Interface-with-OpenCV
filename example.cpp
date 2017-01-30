/*
 * Created on Sat Jan 28 2017 
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

#include <iostream>
#include "libs/camera.h"

static bool run = true;
void command();

int main(void){
    cv::Size size(600,400);
    Camera cam;    //Camera ID 0,
    std::thread commandsThread(command);
    printf("CV Version : %s \n", CV_VERSION); //prints the openCV version
    while(run){
        cv::Mat3b frame = cam.getNewFrame();
        cv::imshow("Camera", frame);
        cv::waitKey(1);
    }
    commandsThread.join();
    return 0;
}

void command(){
    std::string hold_value = " ";
    printf("Press any key to close\n");
    std::cin >> hold_value;	
    run = false;
    return;
}
