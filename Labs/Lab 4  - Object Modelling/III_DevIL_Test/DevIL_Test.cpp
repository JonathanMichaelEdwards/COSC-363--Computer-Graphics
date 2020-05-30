//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: DevIL_Test.cpp
//  See Lab04 (III) for details
//   Input:  Colors.png
//  ========================================================================

#include <iostream>
#include <GL/freeglut.h>
#include <cmath>
#include <IL/il.h>
using namespace std;

GLuint texId;

GLuint loadGLTexture(char* filename)
{
	GLuint imgGLid;
	ILuint imgILid;
	ilGenImages(1, &imgILid);
	glGenTextures(1, &imgGLid); 		// Create texture id
	glBindTexture(GL_TEXTURE_2D, imgGLid);
	ilBindImage(imgILid);   // Binding of DevIL image name
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);   //This is necessay since the default location is upper-left

	if (ilLoadImage(filename))   //if success
	{
		/* Convert image to RGBA */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		/* Create and load textures to OpenGL */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		int imgWidth = ilGetInteger(IL_IMAGE_WIDTH);
		int imgHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
			ilGetData());
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		cout << "Texture successfully loaded. W = " << imgWidth << " H = " << imgHeight <<endl;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	}
	else
	{
		cout << "Couldn't load texture. " <<  endl;
		imgGLid = -1;
	}
	return imgGLid;
}


//---------------------------------------------------------------------
void initialise(void) 
{
	ilInit();
    texId = loadGLTexture("Colors.png");

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glClearColor (1.0, 1.0, 1.0, 1.0);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(30.0, 1.0, 100.0, 5000.0); 
}

//---------------------------------------------------------------------
void display(void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0, 100, 500, 0, 100, 0, 0, 1, 0); 
	glBindTexture(GL_TEXTURE_2D, texId);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);   //A simple quad
	  glTexCoord2f(0, 0);  glVertex3f(-100, 0, 0);
	  glTexCoord2f(1, 0);  glVertex3f(100, 0., 0);
	  glTexCoord2f(1, 1);  glVertex3f(100, 200., 0);
	  glTexCoord2f(0, 1);  glVertex3f(-100, 200, 0);
	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH );
   glutInitWindowSize (700, 700); 
   glutInitWindowPosition (50, 50);

   glutCreateWindow("DevIL test");

   initialise();
   glutDisplayFunc(display); 
 
   glutMainLoop();
   return 0;
}
