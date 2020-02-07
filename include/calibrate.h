#ifndef CALIBRATE_H
#define CALIBRATE_H

#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <cv.h>
#include <highgui.h>

using namespace::cv;


namespace calib{


bool calibrate(cv::Mat img , int board_w , int board_h ,
               cv::Mat rotation ,cv::Mat translation , cv::Mat intrinsic , cv::Mat distCoeffs, bool drawchesscrners = true );



}
#endif
