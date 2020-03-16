//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Boat.cpp
//  See Lab04  for details
//  ========================================================================
 
#include <iostream>
#include <cmath> 
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

GLuint txId;
float angle = 25;  //Rotation angle

//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(1, &txId); 				// Create a Texture object
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId);		//Use this texture
    loadBMP("Fern.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//-- Ground Plane --------------------------------------------------------
void floor()
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glColor3f(0.7, 0.7,  0.7);			//Floor colour

	for(int i = -200; i <= 200; i +=5)
	{
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-200, 0, i);
			glVertex3f(200, 0, i);
			glVertex3f(i, 0, -200);
			glVertex3f(i, 0, 200);
		glEnd();
	}
}

//-------------------------------------------------------------------
void initialise(void) 
{
    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};
	float black[4] = { 0, 0, 0, 1 };

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glEnable(GL_SMOOTH);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, black);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);

    glClearColor (1.0, 1.0, 1.0, 0.0);

	loadTexture();

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
}
//-------------------------------------------------------------------

void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

//-----------------Bottom panel of the boat------------------------
void drawBase()
{
	const int n = 40;   //Size of the array

	float vx[] = { -20, -18, -16, -15, -14, -13, -11, -9, -6, -3, 0, 3, 6, 9, 11, 12, 14, 15, 16, 18, 20,
	                     18,  16,  15,  14,  13,  11,  9,  6,  3,  0,  -3,  -6,  -9, -11, -12, -14, -15, -16, -18};
	float vy[n] = { 0 };
	float vz[] = { 0, 2, 3, 3.5, 4, 4.2, 4.8, 5, 5, 5, 5, 5, 5, 5, 4.8, 4.2, 4, 3.5, 3, 2, 0,
				  -2, -3, -3.5, -4, -4.2, -4.8, -5, -5, -5, -5, -5, -5, -5, -4.8, -4.2, -4, -3.5, -3, -2 };

	//Draw the above polygon here

	

}

//-----------Side+Hull of the boat---------------------------------------
void drawSide()
{
	const int n = 21;   //Size of the array

	float vx[] = { -20, -18, -16, -15, -14, -13, -11, -9, -6, -3, 0, 3, 6, 9, 11, 12, 14, 15, 16, 18, 20};
	float vy[21] = { 0 };
	float vz[] = { 0, 2, 3, 3.5, 4, 4.2, 4.8, 5, 5, 5, 5, 5, 5, 5, 4.8, 4.2, 4, 3.5, 3, 2, 0 };

	float wx[] = { -25, -22, -19, -17, -15, -13,  -11, -9, -6, -3, 0,  3,  6,  9,   11,  13, 15, 17, 19, 22, 25 };
	float wy[] = { 10,  10,  9,  8,  7,  6, 5.5, 5, 5, 5, 5, 5, 5, 5, 5.5, 6, 7, 8, 9, 10, 10 };
	float wz[] = { 0, 3, 5, 6, 6.5, 6.8, 7.2, 7.5, 7.8, 8, 8, 8, 7.8, 7.5, 7.2, 6.8, 6.5, 6, 5, 3, 0 };

	//Draw a quad strip using the above two polygonal lines



}

//-------------------------------------------------------------------
void display(void)
{
	float lgt_pos[]={5.0f, 50.0f, 100.0f, 1.0f};  //light0 position (above the origin) 

	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 25, 100.0, 0., 10., 0., 0., 1., 0.);   

	glRotatef(angle, 0, 1, 0);		//Rotate the entire scene

	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	floor();

	glColor3f (0.0, 0.0, 1.0);    //Used for wireframe mode only
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);


	drawBase();			  //Bottom panel of the boat

	drawSide();           //Front side of the boat

	glFlush();
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
	if(key==GLUT_KEY_LEFT) angle --;
	else if(key==GLUT_KEY_RIGHT) angle ++;

	glutPostRedisplay();
}

//-------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE| GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Country Boat");
   initialise ();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutMainLoop();
   return 0;
}
