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


const float grey[4] = {0.2, 0.2, 0.2, 1.0};
const float white[4]  = {1.0, 1.0, 1.0, 1.0};

/** ------------------------------------------------------------------------------
//  								Initialize OpenGL
// 
//  @param pjtPath - absolute path directory
//  --------------------------------------------------------------------------- */
void initialize(char *pjtPath)
{
    glClearColor(0.23f, 0.38f, 0.47f, 1.0f);  // Background colour
	
	glEnable(GL_LIGHTING);	// Enable lighting

	// Enable light sources
	glEnable(GL_LIGHT0);
	// glEnable(GL_LIGHT1);

	//	Define light's ambient, diffuse, specular properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	// glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  
  
	// glMaterialfv(GL_FRONT, GL_SPECULAR, white);    
  	// glMaterialf(GL_FRONT, GL_SHININESS, 50);

 	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);



	// Load and enable Textures
	// loadTexture(); 
	loadMeshFile("../Models/Cannon.off");	

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60., 1., 1., 500.); 
		
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
	glutTimerFunc(10, ballBounce, 0);  // ball physics
	glutTimerFunc(20, myTimer, 0);     // animate cannon ball
	// glutTimerFunc(1, collBox, 0);   // better performance the other way
	
	glutMainLoop();
	

	return EXIT_SUCCESS;
}
