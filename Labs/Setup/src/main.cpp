// ============================================================================
// 	Author:       Jonathan Edwards
//
// 	Description:  ....
// ============================================================================


// Standard libaries
#include <math.h>

// OpenGL libaries
#include <GL/glut.h>

// Personal libaries
#include "display.h"
#include "peripherals.h"


// Window options
#define WIDTH 600
#define HEIGHT 600
#define WINDOW_POS 10



/** ------------------------------------------------------------------------------
//  								Initialize OpenGL
// 
//  @param pjtPath - absolute path directory
//  --------------------------------------------------------------------------- */
void initialize(char *pjtPath)
{
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	getPath(pjtPath);
}


/** ------------------------------------------------------------------------------
// 						Perform background processing tasks
// 
//  @param delay  - delay in milli seconds
//  --------------------------------------------------------------------------- */
void idle(int delay)
{
	glutPostRedisplay();
	glutTimerFunc(delay, idle, delay);
}


// ----------------------------------------------------------------------------
//         		 Initialize glut window and register call backs
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(WINDOW_POS, WINDOW_POS);
	glutCreateWindow("Template    FPS: ...");
	initialize(argv[0]);

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyEvents);
	glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);

	glutMainLoop();

	return EXIT_SUCCESS;
}
