// ============================================================================
// 	Author:       Jonathan Edwards
//
// 	Description:  ....
// ============================================================================


// Standard libaries
#include <stdio.h>
#include <libgen.h>  
#include <math.h>

// OpenGL libaries
#include <GL/glut.h>

// Personal libaries
#include "display.h"
#include "loadTGA.h"
#include "peripherals.h"



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
		sprintf(title, "Museum    FPS: %.2f", (frameCount / elapsedTime) * FPS_SEC);
		glutSetWindowTitle(title);

		endTime = startTime;
		frameCount = 0;
	}
	frameCount++;
}



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

static bool viewState = 0;
float look_x, look_z=-1, eye_x, eye_z;  //Camera parameters
GLdouble x_view=0,z_view=0, x_view_2=0,z_view_2=0, top_x=0, top_z=-1, top_x_2=0, top_z_2=-1, _zoom_=0;


// 3d person camera
void moveBack(float angle)
{
	eye_x -= 0.5*sin(angle);
	eye_z += 0.5*cos(angle);
}


void moveForward(float angle)
{
	eye_x += 0.5*sin(angle);
	eye_z -= 0.5*cos(angle);
}


void lookRotation(float angle, bool _view)
{
	look_x = eye_x + 200*sin(angle);  // angle the person is looking
	look_z = eye_z - 200*cos(angle);
	viewState = _view;
}
/// --------------------------------------




// top down camera  ----------------------------------------

void zoom(GLdouble _zoom)
{
	_zoom_ += _zoom;
}


void topBottomForward(void)
{
	z_view_2 -= 1;
}


void topBottomBack(void)
{
	z_view_2 += 1;
}


void getView(bool _view)
{
	viewState = _view;
}


void topBottomLeft(void)
{
	x_view_2 += 1;
}


void topBottomRight(void)
{
	x_view_2 -= 1;
}

/// --------------------------------------




// void topBottomView(GLdouble _x, GLdouble _z, bool _view)
// {
// 	x_view += 0.5*sin(_x);
// 	printf("%d\n", _x);
// 	z_view += 0.5*cos(_z);
// 	viewState = _view;
// }


// static bool viewState = 0;


// void moveBack_top(float angle)
// {
// 	x_view -= 0.5*sin(angle);
// 	z_view += 0.5*cos(angle);
// }

// void moveForward_top(float angle)
// {
// 	z_view += 0.5*sin(angle);
// 	z_view -= 0.5*cos(angle);
// }

// void topBottomView(GLdouble _x, GLdouble _z, bool _view)
// {
// 	x_view += 0.5*sin(_x);
// 	z_view -= _z;
// 	viewState = _view;
// }

GLuint txId[2];   //Texture ids
// float angle=0, look_x, look_z=-1., eye_x, eye_z;  //Camera parameters

// load the texture
//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(2, txId); 	// Create 2 texture ids

	glBindTexture(GL_TEXTURE_2D, txId[0]);  //Use this texture
    loadTGA("Wall.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	glBindTexture(GL_TEXTURE_2D, txId[1]);  //Use this texture
    loadTGA("Floor.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	
	
	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


//--------------------------------------------------------------------------------

void boxScene()
{
	glColor3f(0.8, 0.7, 0.3);   //replace with a texture

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]); 

 	glBegin(GL_QUADS);

	////////////////////// BACK WALL ///////////////////////

	// glVertex3f(-15, 1, -15);
	// glVertex3f(-15, -1, -15);
	// glVertex3f(15, -1, -15);
	// glVertex3f(15, 1, -15);

	glTexCoord2f(0.0,  2.0);       glVertex3f(-15, 1, -15);  
	glTexCoord2f(0.0,  0.0);       glVertex3f(-15, -1, -15);  
	glTexCoord2f(12.0, 0.0);       glVertex3f(15, -1, -15);  
	glTexCoord2f(12.0, 2.0);       glVertex3f(15, 1, -15); 



	////////////////////// FRONT WALL ///////////////////////

     glTexCoord2f(0.0,   2.0);       glVertex3f(-15, 1, 15);
 	 glTexCoord2f(0.0,   0.0);       glVertex3f(-15, -1, 15);
 	 glTexCoord2f(12.0,  0.0);       glVertex3f(15, -1, 15);
     glTexCoord2f(12.0,  2.0);       glVertex3f(15, 1, 15);

	////////////////////// LEFT WALL ///////////////////////

	 glTexCoord2f(0.0,  2.0);       glVertex3f(-15, 1, -15);
  	 glTexCoord2f(0.0,  0.0);       glVertex3f(-15, -1, -15);
	 glTexCoord2f(12.0,  0.0);       glVertex3f(-15, -1, 15);
 	 glTexCoord2f(12.0,  2.0);       glVertex3f(-15, 1, 15);


	////////////////////// RIGHT WALL ///////////////////////

	 glTexCoord2f(0.0,  2.0);       glVertex3f(15, 1, -15);
	 glTexCoord2f(0.0,  0.0);       glVertex3f(15, -1, -15);
 	 glTexCoord2f(12.0,  0.0);       glVertex3f(15, -1, 15);
 	 glTexCoord2f(12.0,  2.0);       glVertex3f(15, 1, 15);

	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}
//--------------------------------------------------------------------------------

void floor()
{
    glColor3d(0.6, 0.25, 0.25);  //replace with a texture

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]); // use floor ID 


	glBegin(GL_QUADS);
		 glTexCoord2f(0,  16.0);        glVertex3f(-15, -1, -15);
		 glTexCoord2f(0,  0);        glVertex3f(-15, -1, 15);
		 glTexCoord2f(16.0,  0);        glVertex3f(15, -1, 15);
		 glTexCoord2f(16.0,  16.0);        glVertex3f(15, -1, -15);
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


// Create a single wall
void wall(void)
{
	GLfloat xT=0, yT=-0.25, zT=0;

	glPushMatrix();
		glTranslatef(xT, yT, zT); 
		glScalef(0.3, 1.5, 5);
		glutSolidCube(1);
	glPopMatrix();
}


// construct the outside of the meusem...
void walls(void)
{	
	glColor3f(1, 0.8, 0);  // wall color

	// front left
	glPushMatrix();
		glRotatef(22, 0, 1, 0);
		wall();
	glPopMatrix();

	// front right
	glPushMatrix();
		glRotatef(-22, 0, 1, 0);
		wall();
	glPopMatrix();


	// // back left
	// glPushMatrix();
	// 	glTranslatef(0, 0, -13.815);
	// 	glRotatef(158, 0, 1, 0);
	// 	wall();
	// glPopMatrix();

	
	// // back right
	// glPushMatrix();
	// 	glTranslatef(0, 0, -13.815);
	// 	glRotatef(-158, 0, 1, 0);
	// 	wall();
	// glPopMatrix();
}


void roofTile(void)
{
	GLfloat xT=0, yT=0, zT=0;

	glPushMatrix();
		glColor3f(1, 0.8, 0);
		glTranslatef(xT, yT, zT); 
		glScalef(0.3, 2, 5);
		glutSolidCube(1);
	glPopMatrix();
}


void roof(void)
{
	glColor3f(1, 0, 1);  // wall color

	// front left
	glPushMatrix();
		glTranslatef(-2, 0, 1); 
		glRotatef(110, 0, 0, 1);
		glTranslatef(-2, 0, 1); 
		roofTile();
	glPopMatrix();

	// // front right
	// glPushMatrix();
	// 	glTranslatef(2, 0, 1); 
	// 	glRotatef(-22, 0, 1, 0);
	// 	glTranslatef(-2, 0, -1); 
	// 	roofTile();
	// glPopMatrix();

	// // back left
	// glPushMatrix();
	// 	glTranslatef(12, 0, -14); 
	// 	glRotatef(-22, 0, 1, 0);
	// 	glTranslatef(-12, 0, 14); 
	// 	roofTile();
	// glPopMatrix();

	// // back right
	// glPushMatrix();
	// 	glTranslatef(-12, 0, -14); 
	// 	glRotatef(22, 0, 1, 0);
	// 	glTranslatef(12, 0, 14); 
	// 	roofTile();
	// glPopMatrix();
}


// displays the Muesums External structure 
void displayMuesum()
{

	// move museum walls
	// glPushMatrix();
	// 	glTranslatef(0, 0, 5); 
		walls();
	// glPopMatrix();

	// roof();

}


// change later
// ----------------------------------------------------------------------------
//  				  		Display OpenGL graphics				
// ----------------------------------------------------------------------------
void display(void)  
{
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();
	gluPerspective(45., 1., 0.5, 200.);

	glMatrixMode(GL_MODELVIEW);								
	glLoadIdentity();
	// gluLookAt(eye_x, 20, eye_z,  look_x, 0, look_z,   0, 1, 0);	

	// gluLookAt(0, 30, 0,  0, 0, -1, 0, 1, 0);
	// glTranslatef(z_view, 0, x_view);
	// if (left_top)
	// 	gluLookAt(x_view, 30, z_view,  top_x, 1, top_z, 0, 1, 0);
	// else
	// 	gluLookAt(x_view, 30, z_view,  top_x, 1, top_z, 0, 1, 0);


	// if (left_top) {
	// 	left_top = 0;  // disable button
	// 	glTranslatef(10, 0, 0);
	// 	// gluLookAt(0, 30, 0,  0, 0, -1, 0, 1, 0);
	// } 


	if (viewState) {   // top down view
		glTranslatef(x_view_2, z_view_2, _zoom_);  // since transformed y has become z
		gluLookAt(0, 30, 0,  0, 0, -1, 0, 1, 0);
	} else 
		gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);	
	
	// eye = pos in surroundings
	// look is the rotation

	// boxScene();
	floor();

	displayMuesum();

	
	glutSwapBuffers();	

	showFPS();
}
