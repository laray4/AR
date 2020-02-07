
#include <iostream>
#include <stdio.h>
#include <cv.h>
#include <highgui.h>

#include <calibrate.h>

#define n_boards  1
#define board_dt  20
#define board_w 9
#define board_h 6


#define GL_INTERFACE

#ifdef GL_INTERFACE

#include <glInterface.h>
glInterface _interface;

void init()
{
    _interface.init();
}

void reshape(int w, int h)
{
    _interface.reshape(w, h);
}
void IdleFunc()
{
    _interface.IdleFunc();
}
void display()
{
    _interface.display();
}
void keyboard(unsigned char key, int x, int y)
{
    _interface.keyboard(key, x, y);
}


#endif // GL_INTERFACE




