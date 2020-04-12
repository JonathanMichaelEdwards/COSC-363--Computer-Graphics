// ============================================================================
// Author:       Jonathan Edwards
// Date:         01/31/2020 (V1)
// Description:  Displaying the teapot using the OpenGl Api.
// ==========================================================================*/


#include <stdio.h>
#include <cmath>
#include <libgen.h>  
#include <GL/glut.h>
#include "display.h"
#include "peripherals.h"


// ----------------------------------------------------------------------------
//    Get the path of the file from argv[0] in the main function in main.cpp	
//  
// @param path - The absolute file path to the file being run.				
// ----------------------------------------------------------------------------
char *getPath(char *path) 
{ 
	static char *dirPath = dirname(path);  // Retrieve path excluting file
	return dirPath;
}


// ----------------------------------------------------------------------------
//           				Calculate FPS for Window
// 
//  - Code computes the average frames per second (fps), and also the average			
//    time it takes to render one frame. These stats are appended to the caption 
//    bar window.
// ----------------------------------------------------------------------------
void showFPS(void)
{
	char title[TITLE_SPACE] = {0};
	static double endTime = 0.0;
	static int frameCount = 0;
	
	// Time difference
	double startTime = glutGet(GLUT_ELAPSED_TIME); 
	double elapsedTime = startTime-endTime;

	// Display FPS every 1/4 a second
	if (elapsedTime > QUATER_SEC) {
		sprintf(title, "Teapot    FPS: %.2f", (frameCount / elapsedTime) * FPS_SEC);
		glutSetWindowTitle(title);

		endTime = startTime;
		frameCount = 0;
	}
	frameCount++;
}


// ----------------------------------------------------------------------------
// 						Draws a grid of lines on the floor plane
// ----------------------------------------------------------------------------
void drawFloor(void)
{
	glColor3f(0., 0.5,  0.);			//Floor colour

	for(int i = -50; i <= 50; i ++) {
		glBegin(GL_LINES);			//A set of grid lines on the xz-plane
			glVertex3f(-50, -0.75, i);
			glVertex3f(50, -0.75, i);
			glVertex3f(i, -0.75, -50);
			glVertex3f(i, -0.75, 50);
		glEnd();
	}
}


// ----------------------------------------------------------------------------
//  				  Display OpenGL graphics onto the screen					
// ----------------------------------------------------------------------------
void display(void)  
{
	float lpos[4] = {0., 10., 10., 1.0};  //light's position

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// First three parameters define the position of the Camera
	// Using the Right hand rule
	// (3) middle finger is Z (pointing towards you)
	// (2) 2nd from thumb is y (pointing up)
	// (1) thumb is the x (pointing to the right
	// 
	// Radians to Degrees:
	// deg = rad x 180 / pi
	GLdouble  posX = sin(camCircula(0));
	GLdouble  posZ = cos(camCircula(0));
    gluLookAt(12*posX, camHeight(0), 12*posZ, 0, 0, 0, 0, 1, 0);  //Camera position and orientation

	glLightfv(GL_LIGHT0,GL_POSITION, lpos);   //Set light position

	glDisable(GL_LIGHTING);			//Disable lighting when drawing floor.
    drawFloor();

	glEnable(GL_LIGHTING);			//Enable lighting when drawing the teapot
    glColor3f(0.0, 1.0, 1.0);
    glutSolidTeapot(1.0);

	glFlush(); 
	showFPS();
}
