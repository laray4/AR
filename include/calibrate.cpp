
#include "calibrate.h"

namespace calib{


bool calibrate(cv::Mat image , int board_w , int board_h ,
               cv::Mat rotation ,cv::Mat translation , cv::Mat intrinsic , cv::Mat distCoeffs , bool drawchesscrners  )
{

    //std::cout << "Trying to calibrate......  ";

	int board_n = board_w * board_h;

    Size boardSize(board_w, board_h);

    vector<vector<Point2f> > imagePoints;
    vector<Point2f> pointBuf;
    Size imageSize(image.cols, image.rows);
    const Scalar RED(0,0,255), GREEN(0,255,0);
    const char ESC_KEY = 27;


    bool  found;
    found = findChessboardCorners( image, boardSize, pointBuf,
                CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);

    if ( found == 0 )
            return false;

    Mat imageGray;
    cvtColor(image, imageGray, COLOR_BGR2GRAY);
    cornerSubPix( imageGray, pointBuf, Size(11,11),
                  Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));


    // Draw the corners.
    if (drawchesscrners )
        drawChessboardCorners( image, boardSize, Mat(pointBuf), found );

    imagePoints.push_back(pointBuf);


    vector< vector<Point3f> > objectPoints(1);

    //Fill in chessboard corners coordinates in realworld
    //Respect the order of the corners detected in chessboard and stored in imagePonts
    std::cout << "Corners:" << std::endl;
    for( int j = 0; j < boardSize.width * boardSize.height; ++j )
     {
         Point3f corner( boardSize.width - 1 - j%board_w , j/board_w , 0);
         objectPoints[0].push_back(corner);
         //std::cout << corner << std::endl;
     }


    intrinsic = Mat::eye(3, 3, CV_64F);
    //for  CV_CALIB_FIX_ASPECT_RATIO  use
        intrinsic.at<double>(0,0) = 1.0;
        //intrinsic.at<double>(1,1) = 1.0;

    distCoeffs = Mat::zeros(8, 1, CV_64F);


    vector<Mat> rvecs, tvecs;
    //Find intrinsic and extrinsic camera parameters
    calibrateCamera(objectPoints, imagePoints, imageSize, intrinsic,
                    distCoeffs, rvecs, tvecs , CV_CALIB_FIX_PRINCIPAL_POINT|CV_CALIB_FIX_ASPECT_RATIO|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

    /*
    // In case of distortion in image undistort using computed distortion matrix after calibration
    {
      Mat temp = image.clone();
      undistort(temp, image, intrinsic, distCoeffs);
    }
    //------------------------------ Show image and check for input commands -------------------
    imshow("Image View undistorted", image);
    */

    //First result in rotation and translation matrices (only one chessboard used for calibration)
    cv::Mat rotationMat ;
	cv::Rodrigues(rvecs[0], rotation);
    //copy translation vector to translation matrix
    tvecs[0].copyTo(translation);
	// Save the resulting calibration matrice
	FileStorage fs( "Camera parameters.xml", FileStorage::WRITE );
	fs << "intrinsic"    << intrinsic ;      //std::cout << intrinsic ;
	fs << "distCoeffs"   << distCoeffs ;        //std::cout << distCoeffs ;
	fs << "rotation"     << rotation ;          //std::cout << rotation ;
	fs << "translation"  << translation ;          //std::cout << tvecs[0] ;


    //std::cout << "Calibration done! \n";
	return true;
}


}

