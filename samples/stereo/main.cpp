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
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
//#include <opencv2/gpu/gpu.hpp>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "libs/camera.h"

using namespace cv;
using namespace std;

bool started = true;

void displayImageFromFile(int argc, char** argv);
void command();

int main(int argc, char** argv )
{
    Mat g1, g2;

    Camera cam0(0);
    Camera cam1(1);
    std::thread commandsThread(command);

    //Ptr<StereoBM> sbm = cv::StereoBM::create(128,9); // best so far 128, 9

   // Ptr<StereoBM> sbm;
    //sbm->SADWindowSize = 9;
    
    int temp = 0;
    while(started ){
        /*
        cvtColor(cam1.getFrame(), g1, CV_BGR2GRAY);
        cvtColor(cam0.getFrame(), g2, CV_BGR2GRAY);

        sbm->compute(g1, g2, disp);
        normalize(disp, disp, 0, 255, CV_MINMAX, CV_8U);
        //fastNlMeansDenoising(disp, disp, num_1, 21, 7);

       // blur( disp, disp, Size(2,2) );
    
        imshow("Window", disp);
        waitKey(UPDATE_FREQUENCY);   */
        
        g1 = cam0.getNewFrame();     
        g2 = cam1.getNewFrame();
        imshow("Cam1", g1);
        imshow("Cam2", g2);
        waitKey(1); 
    }

    commandsThread.join();

    return 0;
}

void command(){
    std::string hold_value = " ";
    printf("Press any key to close\n");
    std::cin >> hold_value;	
    started = false;
    return;
}

//takes input from console to open a file
void displayImageFromFile(int argc, char** argv){

    if ( argc != 2 ) {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return;
    }

    Size size(100,100);//the dst image size,e.g.100x100
    Mat image;
    image = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    if ( !image.data )
    {
        printf("No image data \n");
        return;
    }
    resize(image, image,size);//resize image
    namedWindow("Display Image", WINDOW_NORMAL );
    imshow("Display Image", image);
    waitKey(0);
}