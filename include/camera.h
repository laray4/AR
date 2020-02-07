#ifndef CAMERA_H
#define CAMERA_H


#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include <calibrate.h>

class Camera
{
	int board_w;
	int board_h;

	CvMat *rotation;
	CvMat *translation;
	CvMat *intrinsic;

	cv::Mat *rotationMat;
	cv::Mat *translationMat;
	cv::Mat *intrinsicMat;

public:
	Camera( int board_w , int board_h );

    bool calibrate(cv::Mat image , int board_w , int board_h ,
                   cv::Mat rotation ,cv::Mat translation , cv::Mat intrinsic , cv::Mat distCoeffs);

    void saveCameraMats();
};



#endif
