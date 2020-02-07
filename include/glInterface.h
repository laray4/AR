#ifndef GLINTERFACE_H
#define	GLINTERFACE_H


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <cv.h>
#include <highgui.h>
#include <GL/glut.h>
#include <texture.h>
#include <glTools.h>
#include <calibrate.h>

#define DBG

class glInterface{

	int n_bs;
	int b_dt;
	int b_w;
	int b_h;
	bool isCalibrated;


	GLdouble vp_gl[4];
	GLdouble mv_view[16];
	GLdouble proj_view[16];

	cv::Mat img;
	cv::VideoCapture capture;

    cv::Mat intrinsic_matrix;
	cv::Mat translation;
	cv::Mat rotation;
	cv::Mat distCoeffs;

    GLMmodel* pmodel;

	void setOpenGLMatrices( );

public:

	int width , height;

	glInterface(){

	n_bs = 1;
	b_w = 9;
	b_h = 6;
	width = 640;
	height = 480;
	isCalibrated = false;
	pmodel = NULL;
	};

	void draw( bool reported = false );
    void init();
	void IdleFunc();
    void saveCurrentView();
	void background(cv::Mat cvImg);
    void background(char *imf);
    void keyboard(unsigned char key, int x, int y);
    void display(void);
    void reshape(int w, int h);
    void reset();

};



#endif
