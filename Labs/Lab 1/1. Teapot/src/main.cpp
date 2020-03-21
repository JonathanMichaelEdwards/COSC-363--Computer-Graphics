// ============================================================================
// 	Author:       Jonathan Edwards
// 	Date:         02/22/2020 (V1)
// 	Description:  Lab 1. Display a Teapot.
// ============================================================================


#include <GL/glut.h>
#include "display.h"
#include "peripherals.h"


// Window Settings
#define WIDTH 600
#define HEIGHT 600
#define WINDOW_POS 10



// ----------------------------------------------------------------------------
//  								Initialize OpenGL
// ----------------------------------------------------------------------------
void initialize(char *projectPath)
{
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);
	getPath(projectPath);

	glEnable(GL_LIGHTING);		//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 10.0, 1000.0);   //Camera Frustum
}


// ----------------------------------------------------------------------------
// 						Perform background processing tasks
// @param delay - Delay in milli seconds
// ----------------------------------------------------------------------------
void idle(int delay)
{
	glutPostRedisplay();
	glutTimerFunc(delay, idle, delay);
}


// ----------------------------------------------------------------------------
//               Initialize glut window and register call backs
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(WINDOW_POS, WINDOW_POS);
	glutCreateWindow("Teapot    FPS: ...");
	initialize(argv[0]);

	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyEvents);
	glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);

	glutMainLoop();

	return EXIT_SUCCESS;
}
