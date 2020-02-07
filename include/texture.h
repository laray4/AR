/**

Class for loading and image and activate it as a texture for openGL visualisation

*/


#ifndef TEXTURE_H
#define TEXTURE_H


#include <iostream>

#include <GL/glut.h>
#include <QtGui/QtGui>
#include <QtOpenGL/QGLWidget>

#include <cv.h>
#include <highgui.h>


using namespace std;

class Texture {
    GLuint tNum;
public :
    Texture (char *nom) ;
    Texture (IplImage *iplImage) ;
    Texture (cv::Mat cvImage) ;
    void activer() {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tNum);
        //Gestion de la transparence
    }
    void desactiver() {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    ~Texture() {
        glDeleteTextures(1, &tNum); //GEB
    }
    Texture(GLuint larg, GLuint haut) ;
} ;




#endif // TEXTURE_H
