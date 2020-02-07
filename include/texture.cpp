
/* texture.cpp
 *  v1.3 11.10.2005
 */

#include <texture.h>


Texture::Texture (char *nom) {

   glGenTextures(1, &tNum);

   QImage img , img_GL;


   //Load image
   img.load(nom);
   if ( img.isNull())
   {
       cout << nom << "  could not be loaded successfully\n";
       exit(0);
   }

   //convert loaded image to GL format (upstraight image)
   //GL texture coordinate system has y_up axis
   img_GL = QGLWidget::convertToGLFormat(img);


   //Bind the content of the matrix to the texture buffer container
   glBindTexture( GL_TEXTURE_2D, tNum );
   //Define how pixels are stored
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   //Specify the 2D texture image
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,   img_GL.width(), img_GL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_GL.bits() );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    /*
      // parametres de placage de la texture : GL_MODULATE fait un "mix" entre eclairage et texture
      // GL_DECAL utilisait dans le snoopy la composante alpha de la texture.
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      */
    //cout << "Texture created successfully!\n";
}




Texture::Texture (IplImage *iplImage) {

   glGenTextures(1, &tNum);

   QImage img , img_GL;


   //Convert IplImage to QImage
   int height = iplImage->height;
   int width = iplImage->width;
   if (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
     {
         const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
         img = QImage(qImageBuffer, width, height, QImage::Format_RGB888);
         img = img.rgbSwapped();
     }else{
       qWarning() << "Image cannot be converted.";
     }

   /*
   //Load image
   img.load(nom);
   if ( img.isNull())
   {
       cout << nom << "  could not be loaded successfully\n";
       exit(0);
   }
   */

   //convert loaded image to GL format (upstraight image)
   //GL texture coordinate system has y_up axis
   img_GL = QGLWidget::convertToGLFormat(img);


   //Bind the content of the matrix to the texture buffer container
   glBindTexture( GL_TEXTURE_2D, tNum );
   //Define how pixels are stored
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   //Specify the 2D texture image
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,   img_GL.width(), img_GL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_GL.bits() );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    /*
      // parametres de placage de la texture : GL_MODULATE fait un "mix" entre eclairage et texture
      // GL_DECAL utilisait dans le snoopy la composante alpha de la texture.
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      */
    //cout << "Texture created successfully!\n";
}


Texture::Texture (cv::Mat cvImage) {

   glGenTextures(1, &tNum);

   QImage img , img_GL;


   //Convert IplImage to QImage
   int height = cvImage.rows;
   int width = cvImage.cols;
   if (cvImage.type()  == CV_8UC3)
     {
         const uchar *qImageBuffer =(const uchar*)cvImage.data;
         img = QImage(qImageBuffer, width, height, QImage::Format_RGB888);
         img = img.rgbSwapped();
     }else{
       qWarning() << "Image cannot be converted.";
     }


   //convert loaded image to GL format (upstraight image)
   //GL texture coordinate system has y_up axis
   img_GL = QGLWidget::convertToGLFormat(img);


   //Bind the content of the matrix to the texture buffer container
   glBindTexture( GL_TEXTURE_2D, tNum );
   //Define how pixels are stored
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   //Specify the 2D texture image
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,   img_GL.width(), img_GL.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_GL.bits() );

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    /*
      // parametres de placage de la texture : GL_MODULATE fait un "mix" entre eclairage et texture
      // GL_DECAL utilisait dans le snoopy la composante alpha de la texture.
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
      */
    //cout << "Texture created successfully!\n";
}






Texture::Texture(GLuint larg, GLuint haut) {
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      // utilisation de la fonction glCopyTexImage2D
      //char * pixels = new char [3 * larg * haut];
      //glReadPixels (0, 0, larg, haut, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glReadBuffer(GL_FRONT);//GEB

      glGenTextures(1, &tNum);
      // mise en texture courante de textureNames[0]
      glBindTexture(GL_TEXTURE_2D, tNum);
      // indique la maniere dont les octets sont stockés dans "image"
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      // chargement de l'image dans la carte graphique
      //
      // utilisation de la fonction glCopyTexImage2D
      //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, larg, haut,
      //	  GL_RGB, GL_UNSIGNED_BYTE, pixels);
      //delete pixels;
      glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                       0, 0, (GLsizei) (larg), (GLsizei) (haut), 0);

      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      // parametres de placage de la texture : GL_MODULATE fait un "mix" entre eclairage et texture
      // GL_DECAL utilisait dans le snoopy la composante alpha de la texture.
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}


