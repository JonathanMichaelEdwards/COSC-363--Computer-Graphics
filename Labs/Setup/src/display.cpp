// ============================================================================
// 	Author:       Jonathan Edwards
//
// 	Description:  ....
// ============================================================================


// Standard libaries
#include <stdio.h>
#include <libgen.h>  

// OpenGL libaries
#include <GL/glut.h>

// Personal libaries
#include "display.h"



/** -----------------------------------------------------------------------------
//               Get the absolute path directory from the main file
//  
//  @param path    - absolute path directory	
// 
//  @param return  - directory of the absolute path from the main file
//  -------------------------------------------------------------------------- */
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
		sprintf(title, "Template    FPS: %.2f", (frameCount / elapsedTime) * FPS_SEC);
		glutSetWindowTitle(title);

		endTime = startTime;
		frameCount = 0;
	}
	frameCount++;
}


// ----------------------------------------------------------------------------
//  				  		Display OpenGL graphics				
// ----------------------------------------------------------------------------
void display(void)  
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);

	glFlush(); 

	showFPS();
}
