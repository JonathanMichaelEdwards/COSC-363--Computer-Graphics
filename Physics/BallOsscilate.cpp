//  ========================================================================
//  COSC363: Computer Graphics (2020);  University of Canterbury.
//
//  FILE NAME: Cannon.cpp
//  See Lab02.pdf for details
//
//  Program displays the model of a cannon (mesh data from Cannon.off)
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move camera up/down
//  ========================================================================
 
 // Compile and Run:
// - Linux:   g++ -no-pie Cannon.cpp -o Cannon -lGL -lGLU -lglut && ./Cannon
// - Windows: g++ Cannon.cpp -o Cannon -lOpenGL32 -lGLU32 -lfreeGLUT && Cannon

#include <iostream>
#include <fstream>
#include <climits>
#include <math.h> 
#include <GL/freeglut.h>
using namespace std;

//--Globals ---------------------------------------------------------------
float *x, *y, *z;		//vertex coordinate arrays
int *t1, *t2, *t3;		//triangles
int nvrt, ntri;			//total number of vertices and triangles
float angle = -10.0;	//Rotation angle for viewing
float cam_hgt = 100;


// ASCII Keys
#define QUIT    'q'
#define SPACE   0x20
#define ESC     0x1B

#define PI		3.14159265358979323846
#define GRAVITY 9.81

#define velTheta      90
#define _velTheta     (velTheta * PI) / 180
#define airFric       0.35
#define t             1

double initSpeedX = 0;//20; 
double initSpeedY = 10;//16.1264;            // tan(theta) * Vx
double speedX = 0;
double speedY = 0;

bool spacePressed = false;




//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -400; x <= 400; x += 20)
	{
		for(int z = -400; z <= 400; z += 20)
		{
			if(flag) glColor3f(0.6, 1.0, 0.8);
			else glColor3f(0.8, 1.0, 0.6);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+20);
			glVertex3f(x+20, 0, z+20);
			glVertex3f(x+20, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}





// Create and fire a cannon ball as a projectile.
void ball(void)
{
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(velTheta+speedX, 64+speedY, 0);
		glutSolidSphere(5, 36, 18);
	glPopMatrix();
}








//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{
	float lpos[4] = {100., 100., 100., 1.0};  //light's position

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, cam_hgt, 300, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

	drawFloor();

	

	// draw ball
	ball();



	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);	//Background colour

	glEnable(GL_LIGHTING);					//Enable OpenGL states
	glEnable(GL_LIGHT0);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 50, 2000);  //The camera view volume  
}



void rstBall() 
{
	speedX = speedY = 0; 
	initSpeedY = 16.1264; 
	spacePressed = false;
}

// Adding walk animation
void myTimer(int value) 
{  
	// theta += (theta <= -20 ? dir*=-1 : 20 <= theta ? dir*=-1 : dir); 
	if (spacePressed) {
		// if (speedY < -60) {
		// 	speedX += initSpeedX * t * cos(_velTheta);                                  // Speed X stays constant
		// 	speedY += (initSpeedY-=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Speed Y changes due to gravity and air friction
		// } else {
			// speedX += initSpeedX * t * cos(_velTheta);   
		if (speedY > -15) { 
			puts("1");                              // Speed X stays constant
			speedY += (initSpeedY-=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Speed Y changes due to gravity and air friction
		} else if (speedY < -15) {
			speedY += (initSpeedY+=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);
			puts("2");    
		}
	}

	glutPostRedisplay(); 
	glutTimerFunc(30, myTimer, 0); 
}








//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void special(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT) angle--;
    else if(key == GLUT_KEY_RIGHT) angle++;
	else if(key == GLUT_KEY_UP) cam_hgt++;
	else if(key == GLUT_KEY_DOWN) cam_hgt--;


	if(cam_hgt > 200) cam_hgt = 200;
	else if(cam_hgt < 10) cam_hgt = 10;
    
	glutPostRedisplay();
}



// // Adding walk animation
// void myTimer(int value) 
// {  
// 	// theta += (theta <= -20 ? dir*=-1 : 20 <= theta ? dir*=-1 : dir); 
// 	if (spacePressed) {
// 		if (speedY > -64) {
// 			speedX += initSpeedX * t * cos(_velTheta);                                  // Speed X stays constant
// 			speedY += (initSpeedY-=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Speed Y changes due to gravity and air friction
// 		} else resetCannon();
// 	}

// 	glutPostRedisplay(); 
// 	glutTimerFunc(30, myTimer, 0); 
// }




/** ============================================================================
 * 							ASCII key event callback 
 *  ===========================================================================*/
void keyEvents(unsigned char key, int x, int y)
{
    if (key == QUIT) exit(EXIT_SUCCESS);
	else if (key == ESC) exit(EXIT_SUCCESS);

	if (!spacePressed)
		if (key == SPACE) spacePressed = true;

    glutPostRedisplay();
}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Cannon");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(special); 
   glutKeyboardFunc(keyEvents);
   glutTimerFunc(30, myTimer, 0);  // animate humnoid
   glutMainLoop();
   return 0;
}
