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
cv::Mat GetGradient(cv::Mat src_gray);

int main(int argc, char **argv){
    double alpha;
    if(argc != 2) return -1;
    sscanf(argv[1], "%lf", &alpha);
    double beta = (1.0 - alpha);
    Camera cam0(0);    
    Camera cam1(1);
    
    printf("CV Version : %s \n", CV_VERSION); //prints the openCV version

    //get the difference between the two frames, align the images by a known distance
    //assumes the difference does not change while the webcams are active
    cv::Mat3b frame0 = cam0.getNewFrame();
    cv::Mat3b frame1 = cam1.getNewFrame();

    //using this tutorial http://www.learnopencv.com/image-alignment-ecc-in-opencv-c-python/

    //convert to grayscale
    cv::Mat frame0_gray, frame1_gray, warp_matrix;
    cv::cvtColor(frame0, frame0_gray, CV_BGR2GRAY);
    cv::cvtColor(frame1, frame1_gray, CV_BGR2GRAY);
    warp_matrix = cv::Mat::eye(2, 3, CV_32F);

    // Specify the number of iterations.
    int number_of_iterations = 500;
 
    // Specify the threshold of the increment
    // in the correlation coefficient between two iterations
    double termination_eps = 1e-10;
 
    // Define termination criteria
    cv::TermCriteria criteria (cv::TermCriteria::COUNT+cv::TermCriteria::EPS, number_of_iterations, termination_eps);
 
    // Run the ECC algorithm. The results are stored in warp_matrix.
    cv::findTransformECC(frame0_gray, frame1_gray, warp_matrix, cv::MOTION_EUCLIDEAN, criteria);

    std::cout << "Running..." << std::endl;
    std::thread commandsThread(command);

    while(run){
        frame0 = cam0.getNewFrame();
        frame1 = cam1.getNewFrame();
        cv::Mat3b disp, aligned_frame0, aligned_frame1;
        cv::warpAffine(frame0, aligned_frame0, warp_matrix, frame0.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);
        cv::warpAffine(frame1, aligned_frame1, warp_matrix, aligned_frame1.size(), cv::INTER_LINEAR + cv::WARP_INVERSE_MAP);
        //cv::addWeighted(aligned_frame0, alpha, aligned_frame1, alpha, 0.0, disp);
        cv::imshow("Frame 1", aligned_frame1);
        cv::imshow("Frame 0", frame0);
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

cv::Mat GetGradient(cv::Mat src_gray)
{
  cv::Mat grad_x, grad_y;
  cv::Mat abs_grad_x, abs_grad_y;
   
  int scale = 1;
  int delta = 0;
  int ddepth = CV_32FC1; ;
 
  // Calculate the x and y gradients using Sobel operator
  cv::Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
  cv::convertScaleAbs( grad_x, abs_grad_x );
 
  cv::Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);
  cv::convertScaleAbs( grad_y, abs_grad_y );
   
  // Combine the two gradients
  cv::Mat grad;
  cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
 
  return grad; 
 
}