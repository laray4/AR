#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iomanip>


#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>

#include <GL/gl.h>
#include <GL/glut.h>


extern "C" {
#include "glm.h"
}

#define IMG_W 640
#define IMG_h 480

#define ZNEAR 0.01
#define ZFAR 1000.0



using namespace cv;
using namespace std;


namespace cppTools{

void drawstr(GLuint x, GLuint y, char* format, ...);
GLboolean  invert(GLdouble src[16], GLdouble inverse[16]);
void drawaxes(int multicolor = 1 , int n=0 );
void drawCoordinateSystem();
void showCurrentModelviewMatrix();
void showCurrentProjectionMatrix();
void showCurrentViewportArray();


void cvRotationToGL(Mat &rot , GLdouble *r);
void cvTranslationToGL(Mat &trans , GLdouble *t);

void setToIdentity(GLdouble *mat);

void draw_frustum( GLdouble *ModelviewMatrix , GLdouble *ProjectionMatrix ,double w=1, double h=1 , int n=0 , bool drawCamer=false);

void cameraAsCone(double w, double h , bool plan = false);

void printGLMatrix(GLdouble *mat);

void drawmodel(GLMmodel* pmodel, char *);

}
#endif // UTILS_H
