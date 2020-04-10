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
const float orange[4]  = {1, 0.5, 0, 1};
const float white[4]  = {1, 1, 1, 1};

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
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	//	Define light's ambient, diffuse, specular properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, orange);
	glLightfv(GL_LIGHT0, GL_SPECULAR, orange);

	// spotlight LIGHT_1
	glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, orange);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 40); 
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90); 

	// spotlight LIGHT_2 - Guard tourch
	glLightfv(GL_LIGHT2, GL_AMBIENT, grey);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT2, GL_SPECULAR, white);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30); 
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 90); 


	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);  
  
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);    
  	glMaterialf(GL_FRONT, GL_SHININESS, 50);

 	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	// init_particle();

	// Load and enable Textures
	// loadTexture(); 
	// loadMeshFile("../Models/Cannon.off");	

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective(60., WIDTH/HEIGHT, 1., 500.); 
	glMatrixMode(GL_MODELVIEW);
		
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

#include <stdio.h>
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
	glutTimerFunc(20, ballBounce, 0);  // ball physics
	glutTimerFunc(10, cannonBall, 0);     // animate cannon ball

	if (wallHit) // animate the guard
		glutTimerFunc(10, guardAnimation, 0); 
	else
		glutTimerFunc(30, guardAnimation, 0); 
	
	glutMainLoop();
	

	return EXIT_SUCCESS;
}
