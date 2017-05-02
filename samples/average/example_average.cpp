/*
 * Created on Mon Feb 06 2017 
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

int main(int argc, char **argv){
    int numAverages;
    if(argc != 2){
        return -1;
    }
    sscanf(argv[1], "%d", &numAverages);
    Camera cam(0);    //Camera ID 0,
    std::thread commandsThread(command);
    printf("CV Version : %s \n", CV_VERSION); //prints the openCV version
    while(run){
        //cv::Mat3b frame = cam.getAveragedFrame(3); //the average of a number of frames
        cv::Mat3b frame = cam.getNewFrame();
        std::vector<cv::Mat3b> red_frames(numAverages);
        std::vector<cv::Mat3b> blue_frames(numAverages);
        std::vector<cv::Mat3b> green_frames(numAverages);
        std::vector<cv::Mat3b> channels(3);
        cv::split(frame, channels);

        for(int i = 0; i < numAverages; i++){
            red_frames[i] = channels[0];
            blue_frames[i] = channels[1];
            green_frames[i] = channels[2];
        }

        for(int y = 0; y < frame.rows; y++){
            for(int x = 0; x < frame.cols; x++){
                double reds = 0;
                double blues = 0;
                double greens = 0;

                for(int i = 0; i < numAverages; i++){
                    reds += cv::cvGetReal2D(red_frames[i], y, x);
                    greens += cv::cvGetReal2D(green_frames[i], y, x);
                    blues += cv::cvGetReal2D(blue_frames[i], y, x);
                }
                
                reds = reds/numAverages;
                blues = blues/numAverages;
                greens = greens/numAverages;

                cv::cvSetReal2D(channels[0], y, x, reds);
                cv::cvSetReal2D(channels[1], y, x, greens);
                cv::scvSetReal2D(channels[2], y, x, blues);
            }
        }

        cv::Mat3b result;
        cv::merge(channels[0], channels[1], channels[2], result);


        cv::imshow("Camera", result);
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
