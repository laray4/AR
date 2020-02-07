#include <glTools.h>
namespace cppTools{

void drawstr(GLuint x, GLuint y, char* format, ...)
{
    va_list args;
    char buffer[255], *s;

    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    /*
    glRasterPos2i(x, y);
    for (s = buffer; *s; s++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, *s);
    */
}


//Fonction invert de Nate Robins
GLboolean  invert(GLdouble src[16], GLdouble inverse[16])
{
    double t;
    int i, j, k, swap;
    GLdouble tmp[4][4];

    //Set inverse matrix to Identity matrix
    for (int i = 0 ; i < 16 ; i++)
        inverse[i] = (i%5==0)?1:0;


    //Copy source matrix to tmp matrix
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            tmp[i][j] = src[i*4+j];
        }
    }

    for (i = 0; i < 4; i++) {
        /* look for largest element in column. */
        swap = i;
        for (j = i + 1; j < 4; j++) {
            if (fabs(tmp[j][i]) > fabs(tmp[i][i])) {
                swap = j;
            }
        }

        if (swap != i) {
            /* swap rows. */
            for (k = 0; k < 4; k++) {
                t = tmp[i][k];
                tmp[i][k] = tmp[swap][k];
                tmp[swap][k] = t;

                t = inverse[i*4+k];
                inverse[i*4+k] = inverse[swap*4+k];
                inverse[swap*4+k] = t;
            }
        }

        if (tmp[i][i] == 0) {
            /* no non-zero pivot.  the matrix is singular, which
           shouldn't happen.  This means the user gave us a bad
            matrix. */
            return GL_FALSE;
        }

        t = tmp[i][i];
        for (k = 0; k < 4; k++) {
            tmp[i][k] /= t;
            inverse[i*4+k] /= t;
        }
        for (j = 0; j < 4; j++) {
            if (j != i) {
                t = tmp[j][i];
                for (k = 0; k < 4; k++) {
                    tmp[j][k] -= tmp[i][k]*t;
                    inverse[j*4+k] -= inverse[i*4+k]*t;
                }
            }
        }
    }
    return GL_TRUE;
}



void  drawaxes( int multicolor , int n)
{
	if ( multicolor == 1)
		glColor4f(1.0,1.0,0.0,1.0);


    glMatrixMode(GL_MODELVIEW);
    // Coordinate system :  arrows with a Cone at the edges


    glPointSize(5.0);
    glLineWidth(3.0);

	if ( multicolor == 3)
		glColor4f(1.0,0.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_POINTS);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();

	if ( multicolor == 3)
		glColor4f(0.0,1.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.0, 1.0 , 0.0);
    glEnd();
    glBegin(GL_POINTS);
    glVertex4f(0.0, 1.0 , 0.0,1.0);
    glEnd();

	if ( multicolor == 3)
		glColor4f(0.0,0.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    glBegin(GL_POINTS);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glPointSize(1.0);
    glLineWidth(1.0);

    glColor3ub(255, 255, 0);
    glRasterPos3f(1.1, 0.0, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'x');
    glRasterPos3f(0.0, 1.1, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'y');
    glRasterPos3f(0.0, 0.0, 1.1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'z');



	char c[10];
	itoa(n , c ,10);
    glRasterPos3f(0.1, 0.1, 0.0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c[0]);

	/*
	//Drawing a camera like frame
	glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(1.0, 1.0 , 1.0);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(-1.0, 1.0 , 1.0);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(-1.0, 1.0 , 1.0);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(-1.0, -1.0 , 1.0);
    glEnd();
	glColor3f(0.2,0.2,0.2);
	glBegin(GL_QUADS);
    glVertex3f(1.0, 1.0 , 1.0);
    glVertex3f(-1.0, 1.0 , 1.0);
    glVertex3f(-1.0, -1.0 , 1.0);
    glVertex3f(1.0, -1.0 , 1.0);
    glEnd();
	*/
}


void drawCoordinateSystem()
{
    // Coordinate system :  arrows with a Cone at the edges
    glLineWidth(2.0);
    glColor4f(1.0,0.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    //Axis cone edge
    glPushMatrix();
    glTranslatef(1.0 - 0.06 , 0.0, 0.0 );
    glRotated( 90.0 , 0.0 , 1.0 , 0.0 );
    glutSolidCone(0.02 , 0.06, 50 ,  50);
    glPopMatrix();

    glColor4f(0.0,1.0,0.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.0, 1.0 , 0.0);
    glEnd();
    //Axis cone edge
    glPushMatrix();
    glTranslatef(0.0 , 1.0 - 0.06 , 0.0 );
    glRotated( -90.0 , 1.0 , 0.0 , 0.0 );
    glutSolidCone(0.02 , 0.06, 50 ,  50);
    glPopMatrix();

    glColor4f(0.0,0.0,1.0,1.0);
    glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.0, 0.0, 1.0);
    glEnd();
    //Axis cone edge
    glPushMatrix();
    glTranslatef(0.0 , 0.0, 1.0 - 0.06 );
    glutSolidCone(0.02 , 0.06, 50 ,  50);
    glPopMatrix();

    glColor4f(1.0,1.0,1.0,1.0);
    glLineWidth(1.0);

}

void showCurrentModelviewMatrix()
{
    GLdouble curr[16];

    glGetDoublev(GL_MODELVIEW_MATRIX , curr);
    printf("Current Modelview matrix\n");
    /*
    for ( int i = 0 ; i < 16 ; i++)
        printf("\t%f  " , curr[i]);
    */
    printGLMatrix(curr);
    printf("\n");
}


void showCurrentProjectionMatrix()
{
    GLdouble curr[16];

    printf("Current Projection matrix\n");
    glGetDoublev(GL_PROJECTION_MATRIX , curr);
    /*
    for ( int i = 0 ; i < 16 ; i++)
        printf("\t%f  " , curr[i]);
    */
    printGLMatrix(curr);
    printf("\n");

}

void showCurrentViewportArray()
{
    GLint curr[4];

    printf("Current Viewport Array\n");
    glGetIntegerv(GL_VIEWPORT , curr);
    for ( int i = 0 ; i < 4 ; i++)
        printf("\t%d  " , curr[i]);

    printf("\n");

}



void cvRotationToGL(Mat &rot , GLdouble *r)
{
	CV_Assert(rot.cols == 3 && rot.rows == 3 /*&& rot.type == CV_64F*/);

    r[0]  =   rot.at<double>( 0 , 0);
    r[4]  =   rot.at<double>( 0 , 1);
    r[8]  =   rot.at<double>( 0 , 2);

    r[1]  =   rot.at<double>( 1 , 0);
    r[5]  =   rot.at<double>( 1 , 1);
    r[9]  =   rot.at<double>( 1 , 2);

    r[2]  =   rot.at<double>( 2 , 0);
    r[6]  =   rot.at<double>( 2 , 1);
    r[10] =   rot.at<double>( 2 , 2);

    r[12] =   0.0;
    r[13] =   0.0;
    r[14] =   0.0;

    r[3]  = 0.0;
    r[7]  = 0.0;
    r[11] = 0.0;
    r[15] = 1.0;


    return;
}


void cvTranslationToGL(Mat &trans , GLdouble *t)
{
	// trans should be of type CV_64F
	CV_Assert( (trans.cols == 3 || trans.rows == 3 ) /*&& trans.type== CV_64F*/ );

	for ( int i = 0 ; i <  16 ; i++)
    {
        t[i] = (i%5==0)?1:0;
    }

    int rows = trans.rows , cols = trans.cols;

    t[12] =   trans.at<double>( (rows == 1)?0:0 , (cols==1)?0:0 );
    t[13] =   trans.at<double>( (rows == 1)?0:1 , (cols==1)?0:1 );
    t[14] =   trans.at<double>( (rows == 1)?0:2 , (cols==1)?0:2 );

    return;
}


void setToIdentity(GLdouble *mat)
{
    for ( int i = 0 ; i < 16 ; i++)
        mat[i] =(i%5==0)?1:0;

    return;
}






void draw_frustum( GLdouble *ModelviewMatrix , GLdouble *ProjectionMatrix ,double w , double h, int n , bool drawCamera)
{

    GLdouble inverse[16] , temp[16];
    GLdouble  length;

    length =  sqrt(ModelviewMatrix[12]*ModelviewMatrix[12] +
                   ModelviewMatrix[13]*ModelviewMatrix[13] +
                   ModelviewMatrix[14]*ModelviewMatrix[14] );

    for ( int i = 0 ; i < 16 ; i++ )
        temp[i] = ModelviewMatrix[i];




	/**/
    //Invert modelView matrix, to draw camera coordinate system axis
    (int)invert(temp, inverse);

    //Début de NR Code
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

        glMultMatrixd(inverse);

        //draw the viewing frustum
        //drawaxes(3 , n);
		cameraAsCone(w,h , drawCamera);



        //(int)invert( ProjectionMatrix , inverse);

	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}




void cameraAsCone(double w , double h , bool plan)
{
	glColor3f(50,50,50);
	//Drawing a camera as a wire cone
	glBegin(GL_LINES);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.2*w, 0.2*h , -0.2);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(-0.2*w, 0.2*h , -0.2);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(-0.2*w, -0.2*h , -0.2);
    glVertex3f(0.0,0.0, 0.0);           glVertex3f(0.2*w, -0.2*h, -0.2);
    glEnd();

	if (plan)
	{
		glBegin(GL_QUADS);
			glVertex3f(0.2*w, 0.2*h , -0.2);			glTexCoord2d(1.0,.0);
			glVertex3f(-0.2*w, 0.2*h , -0.2);			glTexCoord2d(-1.0,1.0);
			glVertex3f(-0.2*w, -0.2*h , -0.2);			glTexCoord2d(-1.0,-1.0);
			glVertex3f(0.2*w, -0.2*h , -0.2);			glTexCoord2d(1.0,-1.0);
		glEnd();
	}

	else
	{
		glBegin(GL_LINES);
		glVertex3f(0.2*w, 0.2*h , -0.2);	glVertex3f(-0.2*w, -0.2*h , -0.2);
		glVertex3f(-0.2*w, 0.2*h , -0.2);	glVertex3f(0.2*w, -0.2*h, -0.2);

		glVertex3f(0.2*w, 0.2*h , -0.2);	glVertex3f(0.2*w, -0.2*h , -0.2);
		glVertex3f(0.2*w, 0.2*h , -0.2);	glVertex3f(-0.2*w, 0.2*h , -0.2);

		glVertex3f(-0.2*w, -0.2*h , -0.2);	glVertex3f(-0.2*w, 0.2*h, -0.2);
		glVertex3f(-0.2*w, -0.2*h , -0.2);	glVertex3f(0.2*w, -0.2*h, -0.2);
		glEnd();
	}

}


void printGLMatrix(GLdouble *mat)
{
   cout << setw(15)<< mat[0] << setw(15) << mat[4] << setw(15) << mat[8] << setw(15) << mat[12] << endl ;
   cout << setw(15)<< mat[1] << setw(15) << mat[5] << setw(15) << mat[9] << setw(15) << mat[13] << endl ;
   cout << setw(15)<< mat[2] << setw(15) << mat[6] << setw(15) << mat[10] << setw(15) << mat[14]  << endl;
   cout << setw(15)<< mat[3] << setw(15) << mat[7] << setw(15) << mat[11] << setw(15) << mat[15]  << endl;
   cout << endl;
}




void drawmodel(GLMmodel* pmodel , char *path)
{
    if (!pmodel) {
        pmodel = glmReadOBJ(path);
        if (!pmodel) exit(0);
        glmUnitize(pmodel);
        glmFacetNormals(pmodel);
        glmVertexNormals(pmodel, 90.0);
    }

    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}



}//End of namespace definitions
