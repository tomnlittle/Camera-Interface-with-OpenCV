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

#define CAM_RES_X                                   640
#define CAM_RES_Y                                   320

#define FILTER_VALUE                                50
#define FILTER_X                                    CAM_RES_X/FILTER_VALUE
#define FILTER_Y                                    CAM_RES_Y/FILTER_VALUE

bool run;
void command();

int main(int argc, char** argv )
{
    cv::Size size(CAM_RES_X, CAM_RES_Y);

    Camera cam0(0, 0, false, size); // Camera 1
    Camera cam1(1, 0, false, size); // Camera 2

    run = true;     // initialises the run boolean
    std::thread commandsThread(command);    
   
    while(run){
        cv::Mat f1, f2;
        f1 = cam0.getNewFrame();     
        f2 = cam1.getNewFrame();

        //convert the images to grayscale
        cv::cvtColor(f1, f1, CV_BGR2GRAY);
        cv::cvtColor(f2, f2, CV_BGR2GRAY);

        //-- And create the image in which we will save our disparities
        cv::Mat imgDisparity16S = cv::Mat( f1.rows, f1.cols, CV_16S );
        cv::Mat imgDisparity8U = cv::Mat( f1.rows, f1.cols, CV_8UC1 );

        //-- 2. Call the constructor for StereoBM
        int ndisparities = 16*2;   /**< Range of disparity */
        int SADWindowSize = 21; /**< Size of the block window. Must be odd */

        cv::Ptr<cv::StereoBM> sbm = cv::StereoBM::create( ndisparities, SADWindowSize );

        //-- 3. Calculate the disparity image
        sbm->compute( f1, f2, imgDisparity16S );

        //-- Check its extreme values
        double minVal; double maxVal;

        minMaxLoc(imgDisparity16S, &minVal, &maxVal);

        //-- 4. Display it as a CV_8UC1 image
        imgDisparity16S.convertTo( imgDisparity8U, CV_8UC1, 255/(maxVal - minVal));

        cv::Size filterSize(FILTER_X, FILTER_Y); // size of the filter block

        cv::erode(imgDisparity8U, imgDisparity8U, cv::getStructuringElement(cv::MORPH_RECT, filterSize));
        cv::dilate(imgDisparity8U, imgDisparity8U, cv::getStructuringElement(cv::MORPH_RECT, filterSize)); 
        cv::dilate(imgDisparity8U, imgDisparity8U, cv::getStructuringElement(cv::MORPH_RECT, filterSize)); 
        cv::erode(imgDisparity8U, imgDisparity8U, cv::getStructuringElement(cv::MORPH_RECT, filterSize));

        namedWindow( "Disparity Image", cv::WINDOW_NORMAL );
        imshow("Disparity Image", imgDisparity8U );
        imshow("f1", f1);
        cv::waitKey(30); 
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
