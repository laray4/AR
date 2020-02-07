#include "camera.h"

Camera::Camera(int w, int h)
{
	board_w = w;
	board_h = h;
}




bool Camera::calibrate(cv::Mat image , int board_w , int board_h ,
                       cv::Mat rotation ,cv::Mat translation , cv::Mat intrinsic , cv::Mat distCoeffs)
{
   return calib::calibrate(image , board_w , board_h, rotation, translation , intrinsic , distCoeffs);
}


void Camera::saveCameraMats()
{



}
