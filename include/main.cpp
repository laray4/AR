#include <main.h>

#define GL_INTERFACE

int main(int argc, char** argv)
{
#ifndef GL_INTERFACE


	// Allocate Sotrage
	cv::Mat intrinsic_matrix( 3, 3, CV_32FC1 );
	cv::Mat rotation        ( 3, 3, CV_32FC1 );
	cv::Mat translation     ( 1, 3, CV_32FC1 );
	cv::Mat dist_coeff;


	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    Mat frame;
    cap >> frame; // get a new frame from camera


    //frame = cv::imread("save.png" , CV_LOAD_IMAGE_COLOR );
    for(;;)
    {
        imshow("Calibration-view", frame);


        cap >> frame; // get a new frame from camera

        //frame = cv::imread("save.png" , CV_LOAD_IMAGE_COLOR );

		calib::calibrate(frame , board_w , board_h ,rotation , translation, intrinsic_matrix , dist_coeff , false);
        int c = cvWaitKey( 15 );
		if( c == 'p' ){
			c = 0;
			while( c != 'p' && c != 27 ){
				c = cvWaitKey( 250 );
			}
		}
		else if (c == 's')
            cv::imwrite("save.png" , frame);
		if( c == 27 )
            break;

    }

#else

	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(_interface.width, _interface.height);
    glutInitWindowPosition(0 , 0);
    glutCreateWindow("Just for fun :D");

	init();
    glutReshapeFunc(reshape);
	glutIdleFunc(IdleFunc);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
	return 0;

#endif

}
