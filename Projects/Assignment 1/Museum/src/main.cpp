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
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_POS 10



/** ------------------------------------------------------------------------------
//  								Initialize OpenGL
// 
//  @param pjtPath - absolute path directory
//  --------------------------------------------------------------------------- */
void initialize(char *pjtPath)
{
	// Load and enable Textures
	loadTexture();

    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);  // Background colour
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	  // Enable Ambiant and difuse lighting
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  
	glEnable(GL_COLOR_MATERIAL);


	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60., 1., 1., 1000.); 
		
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
   	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(WINDOW_POS, WINDOW_POS);
	glutCreateWindow("Museum    FPS: ...");
	initialize(argv[0]);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyEvents);
	glutSpecialFunc(special);
	glutTimerFunc(TIMER_DELAY, idle, TIMER_DELAY);  // fps counter
	glutTimerFunc(30, ballBounce, 0);  // ball physics
	glutMainLoop();

	return EXIT_SUCCESS;
}
