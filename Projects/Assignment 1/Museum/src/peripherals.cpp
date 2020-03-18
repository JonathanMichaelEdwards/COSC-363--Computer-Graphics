// ============================================================================
// 	Author:       Jonathan Edwards
//
// 	Description:  ....
// ============================================================================


// Standard libaries
#include <stdio.h>

// OpenGL libaries
#include <GL/glut.h>

// Personal libaries
#include "peripherals.h"
#include "display.h"


// ASCII Keys
#define QUIT    'q'
#define ESC     0x1B



/** ------------------------------------------------------------------------------
// 						Perform background processing tasks
//  @param delay - Delay in milli seconds
//  --------------------------------------------------------------------------- */
static void idle(int delay)
{
	glutTimerFunc(delay, idle, delay);
}


// ----------------------------------------------------------------------------
//							Special key event callback 
// ----------------------------------------------------------------------------
// void special(int key, int x, int y)
// {
//     if (key == GLUT_KEY_END) exit(EXIT_SUCCESS);

//     glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);
// }

static int viewState = 0;
float angle=0;
GLdouble angle_top=0;   //x=0,z=0;


static bool left_top = 0,right_top = 0;


void special(int key, int x, int y)
{
	if (viewState) {
		// angle_top++;
		if(key == GLUT_KEY_LEFT) topBottomLeft(1);       //angle_top -= 0.1;  //Change direction
		else if(key == GLUT_KEY_RIGHT) topBottomRight(1); //angle_top += 0.1;
		if(key == GLUT_KEY_DOWN)
			topBottomBack(angle_top);
		else if(key == GLUT_KEY_UP)
			topBottomForward(angle_top);
		

		lookRotationSide(angle_top, viewState);
	} 
	else if (!viewState) {
		if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
		else if(key == GLUT_KEY_RIGHT) angle += 0.1;
		else if(key == GLUT_KEY_DOWN)
		{  //Move backward
			moveBack(angle);
		}
		else if(key == GLUT_KEY_UP)
		{ //Move forward
			// position the person it from the centre
			moveForward(angle);
		}
		lookRotation(angle, viewState);
	}
	
    glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);
}



// ----------------------------------------------------------------------------
//							ASCII key event callback 
// ----------------------------------------------------------------------------
void keyEvents(unsigned char key, int x, int y)
{
	// exit program
    if (key == QUIT) exit(EXIT_SUCCESS);
	else if (key == ESC) exit(EXIT_SUCCESS);

	// signal to change view
	if (key == 'v') {
		viewState++;
		lookRotation(angle, viewState);  // 3d person view
		if (viewState == 2) {
			viewState = 0;
			lookRotationSide(angle_top, viewState);  // top down view
		}
	} 
	else if (key == '+') zoom(1);
	else if (key == '-') zoom(-1);

	glFlush();

    glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);  
}
