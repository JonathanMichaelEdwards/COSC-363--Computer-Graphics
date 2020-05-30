// ============================================================================
//  Author:       Jonathan Edwards
//  Date:         01/31/2020 (V1)
//  Description:  peripherals.cpp contains button events to kill the window.
// ============================================================================


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <GL/glut.h>
#include "peripherals.h"
#include "display.h"


// ASCII Keys
#define   QUIT       0x71  // ASCII keyword 'q'
#define   ESC        0x1B  // ASCII keyword 'ESC'
#define   INC        0.5
#define   DEC       -0.5
#define   ROT_RATE   10.0


// return the new height of the camera
// @param camHgt - whether the camera 
// should increment or decrement
GLdouble camHeight(GLdouble camHgt)
{
	static GLdouble _camHgt = 5.0;  // start cam position
    return (_camHgt += (_camHgt < 2 ? INC : 20 < _camHgt ? DEC : camHgt));
}


// return the new height of the camera
// @param camHgt - whether the camera 
// should increment or decrement
GLdouble camCircula(GLdouble camCirc)
{
	static GLdouble _camCirc = 0;  // start cam position
    return (_camCirc += camCirc);
}


// ----------------------------------------------------------------------------
// 						Perform background processing tasks
// @param delay - Delay in milli seconds
// ----------------------------------------------------------------------------
static void idle(int delay)
{
	glutTimerFunc(delay, idle, delay);
}


// ----------------------------------------------------------------------------
// 							Special key event callback 
// ----------------------------------------------------------------------------
void special(int key, int x, int y)
{
    if (key == GLUT_KEY_END) exit(EXIT_SUCCESS);

    // change height of camera
    if (key == GLUT_KEY_UP)   camHeight(INC);
    else if (key == GLUT_KEY_DOWN) camHeight(DEC);
    
    // Rotate camera along the x-azis
    if (key == GLUT_KEY_LEFT)  camCircula(DEC/ROT_RATE);
    else if (key == GLUT_KEY_RIGHT) camCircula(INC/ROT_RATE);

    glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);
}


// ----------------------------------------------------------------------------
// 							ASCII key event callback 
// ----------------------------------------------------------------------------
void keyEvents(unsigned char key, int x, int y)
{
    if (key == QUIT) exit(EXIT_SUCCESS);
	else if (key == ESC) exit(EXIT_SUCCESS);
}