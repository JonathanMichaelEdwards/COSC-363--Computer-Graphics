/** ============================================================================
 *	Author:       Jonathan Edwards
 *	Date:         01/31/2020 (V1)
 *	Description:  peripherals.cpp contains button events to kill the window.
 *  ==========================================================================*/


#include <stdio.h>
#include <GL/glut.h>
#include "peripherals.h"


// ASCII Keys
#define QUIT    'q'
#define ESC     0x1B




/** ============================================================================
 * 							Special key event callback 
 *  ===========================================================================*/
void special(int key, int x, int y)
{
    if (key == GLUT_KEY_END) exit(EXIT_SUCCESS);
    glutPostRedisplay();
}


/** ============================================================================
 * 							ASCII key event callback 
 *  ===========================================================================*/
void keyEvents(unsigned char key, int x, int y)
{
    if (key == QUIT) exit(EXIT_SUCCESS);
	else if (key == ESC) exit(EXIT_SUCCESS);
    glutPostRedisplay();
}