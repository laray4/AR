

#include "glInterface.h"


void glInterface::setOpenGLMatrices( )
{
    std::cout << std::endl;
    std::cout << "******************************************** \n";
    std::cout << "Set openGL matrices from calibration results \n";
    std::cout << "******************************************** \n";
	//Set viewport matrix according to image size
	vp_gl[0] = 0;  vp_gl[1] = 0;
	vp_gl[2] = img.cols;
	vp_gl[3] = img.rows;


    GLdouble GLtrans[16];
    GLdouble GLrot[16];

    cppTools::cvTranslationToGL(translation , GLtrans);
    cppTools::cvRotationToGL(rotation , GLrot);

    //building modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();
    glMultMatrixd(GLtrans);
    glMultMatrixd(GLrot);
    glGetDoublev(GL_MODELVIEW_MATRIX , mv_view);

    glPopMatrix();

    //  http://spottrlabs.blogspot.fr/2012/07/opencv-and-opengl-not-always-friends.html
    //  we need to adjust a few things to accomodate differences between OpenGL and OpenCV.
    //  First, OpenGL has inverted Y and Z axes relative to OpenGL.
    //  So, our modelView matrix (in OpenGL 1.x parlance) should look like this because
    //  we want to apply it to the points after they go through the [R|t] matrix:


    mv_view[1] *=-1;   mv_view[5] *=-1;   mv_view[9]  *=-1;  mv_view[13] *=-1;
    mv_view[2] *=-1;   mv_view[6] *=-1;   mv_view[10] *=-1;  mv_view[14] *=-1;
	/**/

	cout << "\nModelview Matrix:\n";    cppTools::printGLMatrix(mv_view);


    //////////////////////////////////////////////////////////////////////////////////////////////////

    //Setting projection matrix
    // Reference:  http://jamesgregson.blogspot.fr/2011/11/matching-calibrated-cameras-with-opengl.html

    // Ortho matrix:
    //[      2.0/(R-L),             0,                              0,                -(R+L)/(R-L)]
    //[          0,              2.0/(T-B),                         0,                -(T+B)/(T-B)]
    //[          0,                 0,                          -2.0/(F-N),           -(F+N)/(F-N)]
    //[          0,                 0,                              0,                         1  ]
    //
    //----------------->>>>>    Equivalent to :              glOrtho( L, R , B, T, N, F);            see:    http://msdn.microsoft.com/en-us/library/windows/desktop/dd373965(v=vs.85).aspx

    //tproj =   Camera intrinsic parameters
    double alpha =  intrinsic_matrix.at<double>( 0 , 0 );
    double beta =   intrinsic_matrix.at<double>( 1 , 1 );
    double skew =   intrinsic_matrix.at<double>( 0 , 1 );
    double u0 =     intrinsic_matrix.at<double>( 0 , 2 );
    double v0 =     intrinsic_matrix.at<double>( 1 , 2 );

    // These parameters define the final viewport that is rendered into by
    // the camera.
    double L = 0.0;
    double R = (double) img.cols;
    double T = (double) img.rows;
    double B = 0.0;

    // near and far clipping planes, these only matter for the mapping from
    // world-space z-coordinate into the depth coordinate for OpenGL
    double Near = 0.0001;
    double Far = 10000.0;


    // construct an orthographic matrix which maps from projected
    // coordinates to normalized device coordinates in the range
    // [-1, 1].  OpenGL then maps coordinates in NDC to the current
    // viewport
    Mat ortho;
	ortho = Mat::zeros(4 , 4 , CV_64F);



    ortho.at<double>( 0, 0) = 2.0/(R-L);
    ortho.at<double>( 0, 3) = -(R+L)/(R-L);
    ortho.at<double>( 1, 1) = 2.0/(T-B);
    ortho.at<double>( 1, 3) = -(T+B)/(T-B);
    ortho.at<double>( 2, 2) = -2.0/(Far-Near);
    ortho.at<double>( 2, 3) = -(Far+Near)/(Far-Near);
    ortho.at<double>( 3, 3) = 1.0;
    //cout << "\northo=\n" << ortho;



    // construct a projection matrix, this is identical to the
    // projection matrix computed for the intrinsicx, except an
    // additional row is inserted to map the z-coordinate to
    // OpenGL.
    // The 4x4 OpenGL matrix will be filled this way
    // alpha   s     -x0     0
    // 0      beta   -y0     0
    // 0      0      (N+F)  N*F
    // 0      0       -1     0
    Mat tproj ;
	tproj= Mat::zeros(4 , 4 , CV_64F);

    tproj.at<double>(0,0) = alpha;
    tproj.at<double>(0,1) = skew;
    tproj.at<double>(0,2) = -u0;
    tproj.at<double>(1,1) = beta;
    tproj.at<double>(1,2) = -v0;
    tproj.at<double>(2,2) = (Near+Far);
    tproj.at<double>(2,3) = Near*Far;
    tproj.at<double>(3,2) = -1.0;
    cout << "\nproj=\n" << tproj;


    // resulting OpenGL projectionView is the product of the orthographic
    // mapping to normalized device coordinates and the augmented
    // camera intrinsic matrix
    // Projection matrix = glOrtho * kmatrix
    Mat temp ;
    temp = Mat::zeros(ortho.rows , ortho.cols , CV_64F);

    cv::multiply(ortho , tproj , temp);



    int k = 0 ;
    for (int  j = 0 ; j < ortho.cols ; j++)
        for(int i = 0; i  < ortho.rows ; i++)
        {
            proj_view[k++] = temp.at<double>(i , j);
        }

    proj_view[10] = (double) ( - (Far + Near) / (Far - Near) );
    proj_view[14] = (double) ( - 2 * Far * Near / (Far - Near) );
    proj_view[11] = -1.0;
    cout << "\nProjection Matrix:\n";     cppTools::printGLMatrix(proj_view);


    std::cout << std::endl;
    return;
}


void glInterface::IdleFunc()
{
#ifndef DBG
    this->capture >> this->img;
#else
    this->img = cv::imread("save.png" , CV_LOAD_IMAGE_COLOR );
#endif
	if (
        isCalibrated = calib::calibrate( this->img ,
                                         this->b_w , this->b_h ,
                                         this->rotation ,
                                         this->translation,
                                         this->intrinsic_matrix,
                                         this->distCoeffs)
		)
        {
            this->setOpenGLMatrices();
            std::cout << "Calibration done\n";
        }

	//cv::imshow( "Calibration", this->img );

    glutPostRedisplay();
}

void glInterface::init()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);


	cppTools::setToIdentity(this->proj_view);
	cppTools::setToIdentity(this->mv_view);


	this->rotation          = cv::Mat ( 3 , 3 , CV_64F );
	this->translation       = cv::Mat ( 3 , 1 , CV_64F );
	this->intrinsic_matrix  = cv::Mat ( 3 , 3 , CV_64F );

#ifndef DBG
	 this->capture.open(0); // open the default camera
    if(!this->capture.isOpened())  // check if we succeeded
        return;


    this->capture >> this->img;
#else
    this->img = cv::imread("save.png" , CV_LOAD_IMAGE_COLOR );
#endif

	if (
        isCalibrated = calib::calibrate( this->img ,
                                         this->b_w , this->b_h ,
                                         this->rotation ,
                                         this->translation,
                                         this->intrinsic_matrix,
                                         this->distCoeffs)
		)
        {
            this->setOpenGLMatrices();
        }

    //Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

/*
    GLfloat lightcolor[4] = { 1.0, 0.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightcolor);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcolor);
*/
}

void glInterface::background(cv::Mat cvImg)
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT , vp);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glViewport (vp[0], vp[1],vp[2],vp[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0 , 1 , 0 , 1 , 0, -100 );

    Texture *texture = new Texture(cvImg);

    texture->activer();
    glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex3f(0, 0, 0);
        glTexCoord2d(1.0,0.0); glVertex3f(1, 0, 0);
        glTexCoord2d(1.0,1.0); glVertex3f(1, 1, 0);
        glTexCoord2d(0.0,1.0); glVertex3f(0, 1, 0);
    glEnd();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    texture->desactiver();
    delete texture;

}


void glInterface::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Draw background
	background(this->img);


	//Define viewport
	glViewport(0, 0, (GLsizei) this->width, (GLsizei) this->height);

	//Define projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadMatrixd( this->proj_view );

	//Define the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadMatrixd( this->mv_view );

/*
    if ( isCalibrated )
        cppTools::drawaxes(1,3);
*/


	glPushMatrix();


    //Draw AR object
    GLdouble _size = 3.0;
    glTranslatef((b_w-1)/2. , (b_h-1)/2. , _size/2.);

    glColor3f(255./255.,51./255.,153./255.);
    //Draw cube AR only if calibrated ie. if a chessboard detected and used for calibration
    /*
    if ( isCalibrated )
    */
    glRotatef(90. , 1. , 0. , 0.);
    glScalef (3.,3.,3.);
    if ( isCalibrated )
        cppTools::drawmodel(pmodel , "data-gl/rose+vase.obj");
        //glutSolidSphere(_size , 50. ,50.);
	glPopMatrix();




    //Draw light source position
    GLfloat pos[4] = { 0. , 0. , 0. , 1.};
    glTranslatef(pos[0] , pos[1] , pos[2]);
//    glColor3f(0 , 1. , 0);
//    glutSolidSphere(0.2 , 20 , 20);
    glLightfv(GL_LIGHT0, GL_POSITION, pos);



//    glColor3f(1. , 0. , 0);
//    glutSolidSphere(0.2 , 20 , 20);          //up



	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();





    glutSwapBuffers();
}

void glInterface::reshape(int w, int h)
{
	this->width = w;
	this->height = h;

	//Define viewport
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	//Define projection matrix
	glMatrixMode(GL_PROJECTION);
	//glLoadMatrixd( this->proj_view );
	glLoadIdentity();

	//Define the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixd( this->mv_view );
	glLoadIdentity();

    glutPostRedisplay();
}



void glInterface::background(char *imf)
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT , vp);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glViewport (vp[0], vp[1],vp[2],vp[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0 , 1 , 0 , 1 , 0, 1000 );

    Texture *texture = new Texture(imf);

    texture->activer();
    glBegin(GL_QUADS);
        glTexCoord2d(0.0,0.0); glVertex3f(0, 0, 0);
        glTexCoord2d(1.0,0.0); glVertex3f(1, 0, 0);
        glTexCoord2d(1.0,1.0); glVertex3f(1, 1, 0);
        glTexCoord2d(0.0,1.0); glVertex3f(0, 1, 0);
    glEnd();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    texture->desactiver();
    delete texture;

}


void glInterface::saveCurrentView()
{

    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT , vp);

    cv::Mat captImg ( ((int)vp[3] )/*h*/, ((int)vp[2] )/*w*/ , CV_8UC3);

    // Format BGR ( for opencv cv::Mat storage : 3 channels)
    // Type UUNSIGNED_BYTE for CV_8UC3  format
    glReadPixels(vp[0] , vp[1] , vp[2], vp[3],
                 GL_BGR_EXT, GL_UNSIGNED_BYTE,
                 (GLubyte*)captImg.data);

    std::string str = "current.png";


    // flip vertically the returned texture.  OpenCV and OpenGL coordinate systems are inversed
    cv::Mat captImgFlip (((int)vp[3] ), ((int)vp[2] ) , CV_8UC3);
    cv::flip(captImg, captImgFlip, 0);


    cv::imwrite((char *) str.c_str() , captImgFlip);
    cv::imshow((char *) str.c_str() , captImgFlip);
    cv::waitKey(0);
    cv::destroyAllWindows();

}

void glInterface::reset()
{
	reshape(this->width , this->height);
    glutPostRedisplay();
}


void glInterface::keyboard(unsigned char key, int x, int y)
{

   switch (key) {
      case 27:
		 exit(0);
   case 'r':
	   reset();
       break;
   case 's':
       saveCurrentView();
       break;
   default:
       break;
   }
}
