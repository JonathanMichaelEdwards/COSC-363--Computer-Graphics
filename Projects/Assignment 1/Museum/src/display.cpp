// ============================================================================
// 	Author:       Jonathan Edwards
//
// 	Description:  ....
// ============================================================================


// Standard libaries
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>  
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>


// OpenGL libaries
#include <GL/freeglut.h>
#include <GL/glut.h>

// Personal libaries
#include "display.h"
#include "loadTGA.h"
#include "peripherals.h"




#define M_PI		3.14159265358979323846
/// create wall structure -------------------------------------------------------
#define WALL_ROT_THETA      22
#define WALL_ROT_RAD        (WALL_ROT_THETA*M_PI) / 180
#define RAD_TO_DEG(rad)     (rad*180) / M_PI

#define WALL_SCALE_WIDTH    0.3
#define WALL_SCALE_HEIGHT   3
#define WALL_SCALE_LENGTH   8
#define FLOOR_BED           -1
#define WIDTH_SPACE         9

// move wall in place respect to angles
#define WALL_X     WALL_SCALE_LENGTH*sin(WALL_ROT_RAD)
#define WALL_Z     WALL_SCALE_LENGTH*cos(WALL_ROT_RAD)


const int doorHeight = WALL_SCALE_HEIGHT;
const float doorPointLocal[3] = {0.15, 0, 0};                     // pivot point
const float doorPointGlobal[3] = { };   // diff of point


#define PI		3.14159265358979323846
#define GRAVITY 9.81
#define DRAG_CUBE   1.05  // face  , edge = 0.8

#define FLOOR_X  26
#define FLOOR_Z  26

#define velTheta      90
#define _velTheta     (velTheta * PI) / 180
// #define airFric       0.47



// #define THREADS_BOX_COLL  6








#define t             0.00001   // good slow-motion camera setting for animation       "0.0001" or "0.00001"











#define BOX_SIZE       2
// double boxPosStart[BOX_SIZE] = { 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7 };//, 3.23, 4.34};//, 1.44, 1.55, 1.66, 1.77, 1.88, 1.99 };
// double ballPosY[BOX_SIZE] = { 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7 };
// double conBallPosY[BOX_SIZE] = { 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7 };

double ballPosY[BOX_SIZE][BOX_SIZE] = { 0 };
// double ballPosX[BOX_SIZE] = { 0 };  // pos width
double ballPosX[BOX_SIZE][BOX_SIZE] = { 0 };  // pos width
double ballPosZ[BOX_SIZE][BOX_SIZE] = { 0 };


#define FRAG_BOX_START  2.0



// double boxPosStart[BOX_SIZE] = { 1.0, 1.1, 1.2, 1.3};//, 3.23, 4.34};//, 1.44, 1.55, 1.66, 1.77, 1.88, 1.99 };
// double ballPosY[BOX_SIZE] = { 1.0, 1.1, 1.2, 1.3};

float posRand_X[BOX_SIZE+1][BOX_SIZE+1]= { 0 };

double initSpeedX = 0; //20; 
double initSpeedY = 0; //16.1264;            // tan(theta) * Vx
double speedY[BOX_SIZE][BOX_SIZE] = { 0.001, 0.001 };
bool horizontalCol[BOX_SIZE][BOX_SIZE] = { false };
bool   reset[BOX_SIZE]  = { false };
double speedX = 0;

enum {
	RANGE_OUT = 0,
	RIGHT = 1,
	LEFT = 2
};

// 1 m heigh
// double ballPosY[BOX_SIZE] = { 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 2.7};//, 3.23, 4.34};//, 1.44, 1.55, 1.66, 1.77, 1.88, 1.99 }; // pos height

// double u = ballPosY * sin(_velTheta);  // initial speed

bool _spacePressed = false;
double previous = 0;

bool accDown = true;
bool accUp = false;



#define DIST_Z     200
#define DIST_X     200
#define HEIGHT_Y   160


#define DRAG_SPHERE 0.45 
#define AIR_DENSITY 1.225


pthread_t threads[BOX_SIZE];
pthread_t threadsBoxColl[BOX_SIZE];
pthread_mutex_t *lock;
pthread_mutex_t *lockBoxColl;


int z = 0;
int z_2 = 0;

double da[BOX_SIZE] = { 0 };
int h[BOX_SIZE] = { 0 };
int b[BOX_SIZE] = { 0 };
int c[BOX_SIZE] = { 0 };

// int change = 0;
bool _iState = false; 

bool objCollision[BOX_SIZE] = { false };
bool chkCount[BOX_SIZE][BOX_SIZE] = { false };
bool objStill[BOX_SIZE][BOX_SIZE] = { false };


#define BOX_FRAG_SIZE                   0.1

#define MASS_BOX_PIECE                  .1
#define AREA_BOX_PIECE(size)            size * size * size       // sphere area 4 * PI * (0.1*0.1);
#define V_Terminal(mass, area, drag)    sqrt((2*(mass)*GRAVITY) / (drag*AIR_DENSITY*area)) / 100.

#define GL_CLAMP_TO_EDGE                        0x812F




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




static bool viewState = 0;
float look_x, look_z=-1, eye_x, eye_z=20;  //Camera parameters
GLdouble x_view=0,z_view=0, x_view_2=0,z_view_2=0, top_x=0, top_z=-1, top_x_2=0, top_z_2=-1, _zoom_=0;
  

// 3d person camera  ----------------------------------------
void moveBack(float angle)
{
	float _eye_x = eye_x - 0.5*sin(angle);  // temp pos x
	float _eye_z = eye_z + 0.5*cos(angle);  // temp pos z

	if ((-FLOOR_X <= _eye_x && _eye_x <= FLOOR_X) && (-FLOOR_Z <= _eye_z && _eye_z <= FLOOR_Z)) {
		eye_x = _eye_x;
		eye_z = _eye_z;
	}
}


void moveForward(float angle)
{
	float _eye_x = eye_x + 0.5*sin(angle);  // temp pos x
	float _eye_z = eye_z - 0.5*cos(angle);  // temp pos z

	if ((-FLOOR_X <= _eye_x && _eye_x <= FLOOR_X) && (-FLOOR_Z <= _eye_z && _eye_z <= FLOOR_Z)) {
		eye_x = _eye_x;
		eye_z = _eye_z;
	}
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


#define TEX 6
GLuint txId[TEX] = {0};   //Texture ids


// load textures - skybox
void _loadTGA(const char *_fileLoc, int index)
{
	char loc[100] = {0};

	glBindTexture(GL_TEXTURE_2D, txId[index]);

	sprintf(loc, "../Models/skybox6/%s.tga", _fileLoc);
	loadTGA(loc);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


// load the texture
//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(TEX, txId); 	// Create xxx texture ids

	_loadTGA("front", 0);
	_loadTGA("back", 1);
	_loadTGA("left", 2);
	_loadTGA("right", 3);
	_loadTGA("down", 4);
	_loadTGA("up", 5);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


//--------------------------  sky box ------------------------------------------------------
void back(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[1]); // use floor ID 

	glBegin(GL_QUADS);
	glTexCoord2f(0,  1);       glVertex3f(-DIST_X, -HEIGHT_Y, DIST_Z);
	glTexCoord2f(0,  0);       glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z);
	glTexCoord2f(1, 0);        glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
	glTexCoord2f(1, 1);        glVertex3f(DIST_X, -HEIGHT_Y, DIST_Z); 
	
	glEnd();
}


void front(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[0]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(DIST_X, -HEIGHT_Y, -DIST_Z);  
		glTexCoord2f(0,  0);       glVertex3f(DIST_X, HEIGHT_Y, -DIST_Z);  
		glTexCoord2f(1, 0);        glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);  
		glTexCoord2f(1, 1);        glVertex3f(-DIST_X, -HEIGHT_Y, -DIST_Z); 
	glEnd();
}


void right(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[3]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);      glVertex3f(DIST_X, -HEIGHT_Y, DIST_Z);
		glTexCoord2f(0,  0);      glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(1, 0);       glVertex3f(DIST_X, HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 1);       glVertex3f(DIST_X, -HEIGHT_Y, -DIST_Z);
	glEnd();
}


void left(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[2]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);      glVertex3f(-DIST_X, -HEIGHT_Y, -DIST_Z);
		glTexCoord2f(0,  0);      glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 0);       glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(1, 1);       glVertex3f(-DIST_X, -HEIGHT_Y, DIST_Z);
	glEnd();
}


void floorBed(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[4]); // use floor ID 

	glRotatef(-90, 0, 1, 0);
	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(DIST_X, -HEIGHT_Y, DIST_Z);
		glTexCoord2f(0,  0);       glVertex3f(DIST_X, -HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 0);        glVertex3f(-DIST_X, -HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 1);        glVertex3f(-DIST_X, -HEIGHT_Y, DIST_Z);
	glEnd();
}


void top(void)
{
	glBindTexture(GL_TEXTURE_2D, txId[5]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(DIST_X, HEIGHT_Y,- DIST_Z);
		glTexCoord2f(0,  0);       glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(1, 0);        glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(1, 1);        glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);
	glEnd();
}


// draw and render skybox
void skyBox(void)
{	
    glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glColor3f(1, 1, 1);

		// draw skybox
		back();
		front();
		left();
		right();
		floorBed();
		top();

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

    glPopMatrix();
}
//--------------------------------------------------------------------------------




//----------draw a floor plane-------------------
void drawFloor(void)
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	for(int x = -FLOOR_X; x <= FLOOR_X; x += 2) {
		for(int z = -FLOOR_Z; z <= FLOOR_Z; z += 2) {
			if(flag) glColor3f(1, 1, 1);
			else glColor3f(1, 1, 1);
			glVertex3f(x, FLOOR_BED, z);
			glVertex3f(x, FLOOR_BED, z+2);
			glVertex3f(x+2, FLOOR_BED, z+2);
			glVertex3f(x+2, FLOOR_BED, z);
			flag = !flag;
		}
	}
	glEnd();
}


//// -------------- ball and ground collision -------------------------------



void spacePressed(bool _state)
{
	if (_state) _spacePressed=true;
	else _spacePressed=false;
}



// // draw 2D box
// void _box(int sizeX, int sizeY)
// {
// 	float j = 0;
								
// 	glPushMatrix();
// 		for (float x = 0; x < sizeX; x+=0.1) {
// 			for (float i = 0; i < sizeY; i+=0.1) {
// 				glBegin(GL_TRIANGLES);
// 					glColor3f(0, 0, 1);
// 					// bottom
// 					if (_spacePressed)  // once pressed make sure every piece hits the ground or eachother
// 						j = (rand() % 10) / 10.f;
// 					glVertex3f(-ballPosX/8+0+x-j, j+ballPosY+   0+i, 0);  
// 					glVertex3f(-ballPosX/8+0.1+x-j, j+ballPosY+ 0+i, 0);  
// 					glVertex3f(-ballPosX/8+0+x-j,  j+ballPosY+  0.1+i, 0); 

// 					// top
// 					glColor3f(0, 1, 0);
// 					glVertex3f(ballPosX/8+0.1+x+j, j+ballPosY+  0+i, 0);  
// 					glVertex3f(ballPosX/8+0.1+x+j, j+ballPosY+  0.1+i, 0);  
// 					glVertex3f(ballPosX/8+0+x+j,  j+ballPosY+   0.1+i, 0); 
// 				glEnd();
// 			}
// 		}

// 	glPopMatrix();
// }


// // draw 3d box
// // note: 1x1 default
// void box(void)
// {
// 	// front
// 	glPushMatrix();
// 		_box(1, 1);
// 	glPopMatrix();

// 	// back
// 	glPushMatrix();
// 		glTranslatef(0, 0, -1);
// 		_box(1, 1);
// 	glPopMatrix();

// 	// left
// 	glPushMatrix();
// 		glRotatef(90, 0, 1, 0);
// 		_box(1, 1);
// 	glPopMatrix();

// 	// right
// 	glPushMatrix();
// 		glRotatef(90, 0, 1, 0);
// 		glTranslatef(0, 0, 1);
// 		_box(1, 1);
// 	glPopMatrix();

// 	// bottom
// 	glPushMatrix();
// 		glTranslatef(0, ballPosY, -ballPosY-1);  // shape point bottom left
// 		glRotatef(90, 1, 0, 0);
// 		_box(1, 1);
// 	glPopMatrix();

// 	// top
// 	glPushMatrix();
// 		glTranslatef(0, ballPosY+1, -ballPosY-1);  // shape point bottom left
// 		glRotatef(90, 1, 0, 0);
// 		_box(1, 1);
// 	glPopMatrix();
// }






void rstBall(void) 
{
	_spacePressed = false;
	_iState = false;

	for (int i = 0; i < BOX_SIZE; i++) {
		// ballPosX[i] = 0;
		// ballPosY[i] = boxPosStart[i]; 
		// speedY[i] = 0;
		reset[i] = false;
		objCollision[i] = false;
		// count[i] = 0;
		// chkCount[i] = 0;
	}

	// speedY[BOX_SIZE] = { 0 };
	// speedX = 0;
	// change = 0;  // debouncing check
}



void boxChangeSpeed(int j, int j_2)
{
	if (speedY[z][z_2] <= 0.0) {
		speedY[z][z_2] *= 0.9;//(-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));
	} else 
		speedY[z][z_2] *= -0.9;//(1 - V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));


	if (speedY[j][j_2] <= 0.0) {
		speedY[j][j_2] *= -0.9; //(1 - V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));
	} else 
		speedY[j][j_2] *= 0.9; //(-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));	
}


void boxCollision(int j, int j_2, bool target)
{
	// initial collison detected - only run once while in contact
	// change direction if rising
	if (!chkCount[z][z_2]) {
		if (0 == (int)(speedY[z][z_2]*10000.f)) {
			speedY[z][z_2] = 0;
			// horizontal detection
			// horizontalCol[z][z_2] = true;
			// horizontalCol[j][j_2] = true;

			if (ballPosY[z][z_2] >= FRAG_BOX_START) {
				objStill[z][z_2] = false;
				objStill[j][j_2] = false;
			} else {
				objStill[z][z_2] = true;
				objStill[j][j_2] = true;
			}
		} else {
			// boxChangeSpeed(j, j_2);
			speedY[z][z_2] *= (-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));
			speedY[j][j_2] *= (-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));


			objStill[z][z_2] = false;
			objStill[j][j_2] = false;
		}
		
	// if (speedY[z][z_2] <= 0.0) {
	// 	speedY[z][z_2] *= 0.9;//(-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));
	// } else 
	// 	speedY[z][z_2] *= -0.9;//(1 - V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));

		// change the horizontal speed of x
		// posRand_X[z][z_2] *= -0.9;
		// posRand_X[j][j_2] *= -0.9;

		// speedY[z][z_2] *= -0.9;
		// speedY[j][j_2] *= -0.9;
		// speedY[z][z_2] *= (-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));
		// speedY[j][j_2] *= (-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));

		// if true, hit horizontal face, change x-direction
		if (target) {
			posRand_X[z][z_2] *= -0.9;
			posRand_X[j][j_2] *= -0.9;
		} 

		// only runs once when detected
		ballPosY[z][z_2] += speedY[z][z_2] / (THREADS_BOX_COLL*THREADS_BOX_COLL);
		ballPosY[j][j_2] += speedY[j][j_2] / (THREADS_BOX_COLL*THREADS_BOX_COLL);

		// ballPosX[z][z_2] += posRand_X[z][z_2];
		// ballPosX[j][j_2] += posRand_X[j][j_2];
	}
}


// if y-dir found, then change x-dir
void checkPosX(int j, int j_2, bool *target)
{
	if ((ballPosY[z][z_2]-0.05) <= (ballPosY[j][j_2]+0.05) && (ballPosY[j][j_2]-0.05) <= (ballPosY[z][z_2]+0.05))
		*target = true;  // x-dir move
	else
		*target = false;

	// posRand_X[z][z_2] *= -0.9;
	// posRand_X[j][j_2] *= -0.9;
		
	// if ((ballPosX[z][z_2]-0.05) <= (ballPosX[j][j_2]+0.05) && (ballPosY[z][z_2]-0.05) <= (ballPosY[j][j_2]+0.05) && (ballPosY[j][j_2]-0.05) <= (ballPosY[z][z_2]+0.05) && horizontalCol[z][z_2] == RANGE_OUT) { // if 'j' triggered ('z' came in from right side)
	// 	// puts("1");
	// 	horizontalCol[z][z_2] = RANGE_OUT;
	// 	// horizontalCol[j][j_2] = true;
	// } else {
	// 	horizontalCol[z][z_2] = RIGHT;
	// 	// puts("Out");
	// }
	
	// if ((ballPosX[j][j_2]-0.05) <= (ballPosX[z][z_2]+0.05) && (ballPosY[z][z_2]-0.05) <= (ballPosY[j][j_2]+0.05) && (ballPosY[j][j_2]-0.05) <= (ballPosY[z][z_2]+0.05)) { // if 'j' triggered ('z' came in from left side)
	// 	puts("2");
	// 	horizontalCol[z][z_2] = LEFT;
	// }
}


void *_boxDetectBoxCollision(void *arg)
{
	pthread_mutex_lock(lockBoxColl);

	static bool target = false;

	// check to see if current box fragment hits anyothers
	for (int j = 0; j < BOX_SIZE; j++) {  // row  (check current row on with every row of the cube)
		for (int j_2 = 0; j_2 < BOX_SIZE; j_2++) {
			if (j == z && j_2 == z_2) break;
			if ((ballPosX[z][z_2]-0.05) <= (ballPosX[j][j_2]+0.05) && (ballPosX[j][j_2]-0.05) <= (ballPosX[z][z_2]+0.05)) {
				if ((ballPosY[z][z_2]-0.05) <= (ballPosY[j][j_2]+0.05) && (ballPosY[j][j_2]-0.05) <= (ballPosY[z][z_2]+0.05)) { 
					if ((ballPosZ[z][z_2]-0.05) <= (ballPosZ[j][j_2]+0.05) && (ballPosZ[j][j_2]-0.05) <= (ballPosZ[z][z_2]+0.05)) {
						boxCollision(j, j_2, target);
						
						chkCount[z][z_2] = true;
						chkCount[j][z_2] = true;
					}
				} 
			} else {
				// checkPosX(j, j_2);

				// horizontalCol[z][z_2] = RANGE_OUT;
				chkCount[z][z_2] = false;
				chkCount[j][z_2] = false;
				// increase falling space between objects
				ballPosY[j][j_2] += ((speedY[j][j_2]/BOX_SIZE) / ((THREADS_BOX_COLL*THREADS_BOX_COLL*THREADS_BOX_BOX_COLL)));  // iterates in relation to box size
			}

			if (!chkCount[z][z_2] && !chkCount[j][z_2])
				checkPosX(j, j_2, &target);
		}
	}


	pthread_mutex_unlock(lockBoxColl);

	return NULL;
}


pthread_mutex_t _lockBoxColl = PTHREAD_MUTEX_INITIALIZER;  // thread safe
void boxDetectBoxCollision()
{

	// Linux
	for (int i = 0; i < THREADS_BOX_BOX_COLL; i++) {
		lockBoxColl = &_lockBoxColl;
		// boxColl[i] = i;
		pthread_create(&threadsBoxColl[i], NULL, _boxDetectBoxCollision, NULL);
	}

	for (int i = 0; i < THREADS_BOX_BOX_COLL; i++) 
		pthread_join(threadsBoxColl[i], NULL);
}


float zDir[20] = { 0.f };
void boxDetectGroundCollision()
{
	// box piece's collision with floor
	// if current pos y plus last speed is greater then floor bed, calculate new pos due to speed and direction
	if (!chkCount[z][z_2]) {
		if (ballPosY[z][z_2]+speedY[z][z_2] >= FLOOR_BED) {
			speedY[z][z_2] -= (sin(_velTheta) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t));
			objStill[z][z_2] = false;
		} else { // hit the floor
			if (0 == (int)(speedY[z][z_2]*1000.f)) { // use boolen expression to register
				speedY[z][z_2] = 0;
				// horizontalCol[z][z_2] = true;
				objStill[z][z_2] = true;
			} else {
				// boxDetectBoxCollision();
				speedY[z][z_2] *= (-1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(BOX_FRAG_SIZE), DRAG_CUBE));  // resistance percentage 
				objStill[z][z_2] = false;
			}
		}
		
		ballPosY[z][z_2] += speedY[z][z_2] / ((THREADS_BOX_COLL*THREADS_BOX_COLL));
	}
}


void *floorCollisionBOX(void *arg)
{
	// z = *(int*)arg;  // block number
	pthread_mutex_lock(lock);


	for (int n = 0; n < 1; n++) {               // rows
		for (int n_2 = 0; n_2 < BOX_SIZE; n_2++) {     // cols

			z = n;
			z_2 = n_2;

			// if (!objStill[n][n_2]) {
				boxDetectGroundCollision();
				boxDetectBoxCollision();
			// }
		}
	}


	pthread_mutex_unlock(lock);

    return NULL;
}



pthread_mutex_t _lock = PTHREAD_MUTEX_INITIALIZER;  // thread safe
void collBox(int value) 
{  
	static bool _threadStop = false;


	if (_spacePressed) {  

		// if thread is false trigger to stop
		for (int i = 0; i < BOX_SIZE; i++) {
			for (int j = 0; j < BOX_SIZE; j++) {
				if (!objStill[i][j]) {
					_threadStop = false;
					break;
				}
				_threadStop = true;  // all boxes are true - complete (stop and reset)
			}
		}
			
		// if (!_threadStop) {
			// workers
			for (int i = 0; i < THREADS_BOX_COLL; i++) {
				lock = &_lock;
				b[i] = i;
				pthread_create(&threads[i], NULL, floorCollisionBOX, &b[i]);
			}

			for (int i = 0; i < THREADS_BOX_COLL; i++) 
				pthread_join(threads[i], NULL);
		// }
	}
	// glutTimerFunc(1, collBox, 0); 
}


void _boxCube(void)
{
	glPushMatrix();
		glScalef(0.1, 0.1, 0.1);
		glutSolidCube(1);
	glPopMatrix();
}


void _cube3D(int row, int col)
{
	double val = 0;
	static int iRand[BOX_SIZE+1][BOX_SIZE+1] = { 0 };
	// static float posRand_X[BOX_SIZE+1][BOX_SIZE+1]= { 0 };
	static float posRand_Z[BOX_SIZE+1][BOX_SIZE+1]= { 0 };
	static double _fri[BOX_SIZE+1][BOX_SIZE+1] = { 0 };
	bool posRand[2] = { false };  // x, z

	// get random values when btn pressed - run once
	if (!_iState) {
		// keep finding random values every clock tick unitl btn pressed
		for (int i = 0; i < BOX_SIZE; i++) {
			for (int j = 0; j < BOX_SIZE; j++) {
				iRand[i][j] = rand() % BOX_SIZE;	
				posRand_X[i][j] = (float)(rand() % 10) / 2000.f;
				// posRand_Z[i][j] = (float)(rand() % 10) / 2000.f;
			}
		}

		// posRand_X[0][0] = 0.001;
		// posRand_X[0][1] = -0.001;

		// posRand_X[1][0] = 0.001;
		// posRand_X[1][1] = 0;


		// run once when btn pressed
		if (_spacePressed) {
			// for (int i = 0; i < BOX_SIZE; i++)
			// 	for (int j = 0; j < BOX_SIZE; j++)
			// 		ballPosX[row][col] += posRand_X[i][j];

			_iState = true;
		}
	}

	if (_spacePressed) {
		// // for (int i = 0; i < BOX_SIZE; i++)
		// // 	for (int j = 0; j < BOX_SIZE; j++)
		// if (_fri[row][col] >= -0.005) {
		// 	posRand_X[row][col] = 0;
		// 	// _fri[row][col] = 0;
		// } else
		// 	// _fri[row][col] -= 0.00001;

		// ballPosX[row][col] += posRand_X[row][col] + _fri[row][col];

		if (0 == speedY[row][col]) {
			if ((int)(posRand_X[row][col]*100000.f) == 0)
				posRand_X[row][col] = 0;
			else if (posRand_X[row][col] > 0)
				posRand_X[row][col] -= 0.00001;
			else
				posRand_X[row][col] += 0.00001;
				
			ballPosX[row][col] += posRand_X[row][col];
		} else 
			ballPosX[row][col] += posRand_X[row][col];
	}

#if 0
	if (_spacePressed) {
		// if (0 == (int)(posRand_X[iRand[row][col]][iRand[row][col]] *10000.f)) {
		// 	posRand_X[iRand[row][col]][iRand[row][col]] = 0;
		// 	posRand[0] = true;
		// } else {
		// 	posRand[0] = false;
		// }
			
		// if (0 == (int)(posRand_Z[iRand[row][col]][iRand[row][col]] *10000.f)) {
		// 	posRand_Z[iRand[row][col]][iRand[row][col]] = 0;
		// 	posRand[1] = true;
		// } else {
		// 	posRand[1] = false;
		// }






		// stop ball from moving when stationary
		if (0 != speedY[row][col] && !objStill[row][col] && !horizontalCol[row][col]) {
			ballPosX[row][col] += posRand_X[iRand[row][col]][iRand[row][col]];
			ballPosZ[row][col] += posRand_Z[iRand[row][col]][iRand[row][col]];
		} else if (objStill[row][col] && horizontalCol[row][col]) {		

			// if (ballPosX[row][col] <= 0)
			// 	ballPosX[row][col] *= -0.2;
			// else 
			// 	ballPosX[row][col] *= 0.2;

			// if (ballPosZ[row][col] <= 0)
			// 	ballPosZ[row][col] *= -0.2;
			// else 
			// 	ballPosZ[row][col] *= 0.2;
			speedY[row][col] = 0;

			// if (!posRand[0]) {
				if (ballPosX[row][col] <= 0)
					posRand_X[iRand[row][col]][iRand[row][col]] *= -0.5;
				else 
					posRand_X[iRand[row][col]][iRand[row][col]] *= 0.5;
			// }

			// if (!posRand[1]) {
				if (ballPosZ[row][col] <= 0)
					posRand_Z[iRand[row][col]][iRand[row][col]] *= -0.5;
				else 
					posRand_Z[iRand[row][col]][iRand[row][col]] *= 0.5;
			// }

			ballPosX[row][col] += posRand_X[iRand[row][col]][iRand[row][col]];
			ballPosZ[row][col] += posRand_Z[iRand[row][col]][iRand[row][col]];
		}














		// // stop ball from moving when stationary
		// if (0 != speedY[row][col] && !objStill[row][col] && !horizontalCol[row][col]) {
		// 	ballPosX[row][col] += posRand_X[iRand[row][col]][iRand[row][col]];
		// 	ballPosZ[row][col] += posRand_Z[iRand[row][col]][iRand[row][col]];
		// } else if (objStill[row][col] && horizontalCol[row][col]) {		

		// 	// if (ballPosX[row][col] <= 0)
		// 	// 	ballPosX[row][col] *= -0.2;
		// 	// else 
		// 	// 	ballPosX[row][col] *= 0.2;

		// 	// if (ballPosZ[row][col] <= 0)
		// 	// 	ballPosZ[row][col] *= -0.2;
		// 	// else 
		// 	// 	ballPosZ[row][col] *= 0.2;
		// 	speedY[row][col] = 0;

		// 	// if (!posRand[0]) {
		// 		if (ballPosX[row][col] <= 0)
		// 			posRand_X[iRand[row][col]][iRand[row][col]] *= -0.5;
		// 		else 
		// 			posRand_X[iRand[row][col]][iRand[row][col]] *= 0.5;
		// 	// }

		// 	// if (!posRand[1]) {
		// 		if (ballPosZ[row][col] <= 0)
		// 			posRand_Z[iRand[row][col]][iRand[row][col]] *= -0.5;
		// 		else 
		// 			posRand_Z[iRand[row][col]][iRand[row][col]] *= 0.5;
		// 	// }

		// 	ballPosX[row][col] += posRand_X[iRand[row][col]][iRand[row][col]];
		// 	ballPosZ[row][col] += posRand_Z[iRand[row][col]][iRand[row][col]];
		// }
	}



#endif

	// for (float x = 0; x < 0.1; x+=0.1) { //(BOX_SIZE/10.f); x+=0.1) {
		glPushMatrix();	
			glColor3f(0, 1, 0);
			glTranslatef(ballPosX[row][col], ballPosY[row][col] +0.25, ballPosZ[row][col]);

			// rotate box with respect to its position
			val = 0;//RAD_TO_DEG(atan(ballPosZ[row][col]/ballPosX[row][col]));
			if (_spacePressed) glRotatef(val, 0, 1, 0); 
			_boxCube();
		glPopMatrix();
	// }
}


void boxCube(void)
{
	static bool initPos = false;

	if (!initPos) {
		// double _ballPosY[BOX_SIZE] = { 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7 };
		// double _ballPosX[BOX_SIZE] = { 0, 1 , 2, 3, 4, 5, 6, 7};

		double _ballPosY[BOX_SIZE] = { 0, 1 };
		double _ballPosX[BOX_SIZE] = { -0.5, 0.8};

		// build the box out of multiple cubes
		for (int i = 0; i < BOX_SIZE; i++) {            // row
			for (int j = 0; j < BOX_SIZE; j++) {        // col
				ballPosX[i][j] = _ballPosX[j];
				ballPosY[i][j] = _ballPosY[i];
			}
		}
		
		initPos = true;
	}

	for (int y = 0; y < BOX_SIZE; y+=1) {
		for (int col = 0; col < BOX_SIZE; col+=1) {
			_cube3D(y, col);
		}
	}



	// glPushMatrix();	
	// 	glColor3f(1, 0, 0);

	// 	// if (_spacePressed) glRotatef(-30, 0, 1, 0); 
	// 	glTranslatef(-1.58+0.2, -0.8, -0.91+0.05);
	// 	if (_spacePressed) glRotatef(-30, 0, 1, 0); 
	// 	_boxCube();
	// glPopMatrix();

	// glPushMatrix();	
	// 	glColor3f(0, 0, 1);
	// 	glTranslatef(-1.57999+0.24, -0.8, -0.91220765);
	// 	if (_spacePressed) glRotatef(-30, 0, 1, 0); 
	// 	_boxCube();
	// glPopMatrix();


		// _cube3D(0, 0);
		// _cube3D(1, 1);
		// _iState = false;
		// _cube3D(1, false);

	// // get random values
	// if (!(_iState))
	// 	for (int i = 0; i < 20; i++)
	// 		rot[i] = rand() % 180;
	// if (_spacePressed) _iState = true;

	// for (float x = 0; x <= 1; x+=0.1) {
	// 		glPushMatrix();	
	// 			glColor3f(0, 1, 0);

	// 			if (_spacePressed) glRotatef(rot[(int)(x*10)], 0, 1, 0);
	// 			glTranslatef(-ballPosX/8+x, ballPosY[0]+0.25+y, 0);
	// 			_boxCube();
	// 		glPopMatrix();
	// }


	// get random values
	// if (!(_iState))
	// 	for (int i = 0; i < 4; i++)
	// 		rot[i] = rand() % 40;
	// if (_spacePressed) _iState = true;

	// // bottom left
	// glPushMatrix();	
	// 	glColor3f(1, 0, 0);

	// 	if (_spacePressed) glRotatef(-rot[0], 0, 1, 0);
	// 	glTranslatef(-ballPosX/8, ballPosY[0]+0.25, 0);
	// 	_boxCube();
	// glPopMatrix();

	// // top left
	// glPushMatrix();	
	// 	glColor3f(0, 1, 0);

	// 	if (_spacePressed) glRotatef(rot[1], 0, 1, 0);
	// 	glTranslatef(-ballPosX/8, ballPosY[1]+0.25, 0);
	// 	_boxCube();
	// glPopMatrix();

	// // bottom right
	// glPushMatrix();	
	// 	glColor3f(0, 1, 0);

	// 	if (_spacePressed) glRotatef(-rot[2], 0, 1, 0);
	// 	glTranslatef(ballPosX/8 + 0.5, ballPosY[0]+0.25, 0);
	// 	_boxCube();
	// glPopMatrix();

	// // top right
	// glPushMatrix();	
	// 	glColor3f(1, 0, 0);
	// 	if (_spacePressed) glRotatef(rot[3], 0, 1, 0);
	// 	glTranslatef(ballPosX/8 + 0.5, ballPosY[1]+0.25, 0);
	// 	_boxCube();
	// glPopMatrix();
}




// #define BALL_RADIUS 0.1

// void ball(void)
// {
// 	glPushMatrix();
// 		glColor3f(0, 0, 1);
// 		glTranslatef(ballPosX, ballPosY, 0);
// 		glutSolidSphere(BALL_RADIUS, 36, 18);
// 	glPopMatrix();
// }


// perfect elastic collision
// void bounceUpAndDown(int value) 
// {  
	// if (_spacePressed) {  
	// 	if (ballPosY+speedY >= FLOOR_BED+BALL_RADIUS)
	// 		speedY -= sin(_velTheta) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // acceleration (drag)
	// 	else 
	// 		speedY *= -1;                                                            // change direction

	// 	ballPosY += speedY;
	// }

// 	glutTimerFunc(30, bounceUpAndDown, 0); 
// }


// #define DRAG_SPHERE 0.45 
// #define AIR_DENSITY 1.225

// bool reset = false;



// void rstBall(void) 
// {
// 	ballPosY = 1; 
// 	ballPosX = 0;
// 	speedY = 0;
// 	speedX = 0;
// 	_spacePressed = false;
// 	reset = false;
// }


// void ballBounce(int value) 
// {  
// 	double mass = 5.;
// 	double area = 4 * PI * (BALL_RADIUS*BALL_RADIUS);
// 	double V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(MASS_BOX_PIECE), DRAG_CUBE)  = sqrt((2*(mass)*GRAVITY) / (DRAG_SPHERE*AIR_DENSITY*area)) / 100; 

// 	if (_spacePressed) {  
// 		if (ballPosY+speedY >= FLOOR_BED+BALL_RADIUS) {
// 			reset = false;
// 			speedY -=  sin(_velTheta) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Gravity acceleration movement (drag)
// 			ballPosX += 0.1;  // x-direction movement
// 		} else {
// 			if (reset) rstBall();
// 			reset = true;
// 			speedY *= -1 + V_Terminal(MASS_BOX_PIECE, AREA_BOX_PIECE(MASS_BOX_PIECE), DRAG_CUBE) ;  // resistance percentage
// 		}
// 		ballPosY += speedY;
// 	}
	
// 	glutTimerFunc(10, ballBounce, 0); 
// }




// Create a single wall
void wall(void)
{
	glPushMatrix();
		// move the wall to the ground level
		glTranslatef(0, (WALL_SCALE_HEIGHT/2)-1, 0);        // y = -0.25  
		glScalef(WALL_SCALE_WIDTH, WALL_SCALE_HEIGHT, WALL_SCALE_LENGTH);
		glutSolidCube(1);
	glPopMatrix();
}


void openDoor(void)
{
	glTranslatef(0, (WALL_SCALE_HEIGHT/2)-1, 0);        // y = -0.25  
	glScalef(WALL_SCALE_WIDTH, WALL_SCALE_HEIGHT, WALL_SCALE_LENGTH/2.0f);
	glutSolidCube(1);
}


void sideWall(void)
{
	glTranslatef(0, (WALL_SCALE_HEIGHT/2)-1, 0);        // y = -0.25  
	glScalef(WALL_SCALE_WIDTH, WALL_SCALE_HEIGHT, WALL_SCALE_LENGTH+0.3);
	glutSolidCube(1);
}

// front and back
void sideWalls()
{
	// back wall
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(0, 0, -WALL_Z-WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		glScalef(1, 1, 2.5);
		sideWall();
	glPopMatrix();
	

	// front left
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef((-WALL_X*2)-0.2, 0, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();
	

	// front right
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef((WALL_X*2)+0.2, 0, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();


	// front door - opens anti-clockwise
	glPushMatrix();
		glColor3f(0, 1, 0);

		glTranslatef(2, 0, 2 +2);
		glRotatef(130, 0, 1, 0);
		glTranslatef(-2, 0, -2 -2);
 
		glTranslatef(2, 0, 2);

		// glRotatef(90, 0, 1, 0);
		openDoor();
	glPopMatrix(); 
}



		

// side walls
// construct the outside of the meusem...
void walls(void)
{	
	// front left
	glPushMatrix();
		glColor3f(1, 0.8, 0);
		glTranslatef(-WALL_X-WIDTH_SPACE, 0, 0);        // move left
		glRotatef(WALL_ROT_THETA, 0, 1, 0);
		wall();
	glPopMatrix();

	// front right
	glPushMatrix();
		glColor3f(1, 0.8, 0);
		glTranslatef(WALL_X+WIDTH_SPACE, 0, 0);    // move right
		glRotatef(-WALL_ROT_THETA, 0, 1, 0);
		wall();
	glPopMatrix();


	// back left
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(-WALL_X-WIDTH_SPACE, 0, -WALL_Z);
		glRotatef(-WALL_ROT_THETA, 0, 1, 0);
		wall();
	glPopMatrix();

	
	// back right
	glPushMatrix();
		glColor3f(1, 0, 0);
		glTranslatef(WALL_X+WIDTH_SPACE, 0, -WALL_Z);
		glRotatef(WALL_ROT_THETA, 0, 1, 0);
		wall();
	glPopMatrix();

	sideWalls();
}

// -------------------------------------------------------






/// create roof structure -------------------------------------------------------
#define ROOF_ROT_THETA      45
#define ROOF_ROT_RAD        (ROOF_ROT_THETA*M_PI) / 180
#define ROOF_SCALE_WIDTH    0.3
#define ROOF_SCALE_HEIGHT   3
#define ROOF_SCALE_LENGTH   8

const int height = ROOF_SCALE_HEIGHT;
const float pointLocal[3] = { 0, -1, 0 };                     // pivot point
const float pointGlobal[3] = { -ROOF_SCALE_WIDTH/2, 0, 0 };   // diff of point



void roofTile(void)
{
	glPushMatrix();
		glScalef(ROOF_SCALE_WIDTH, ROOF_SCALE_HEIGHT, ROOF_SCALE_LENGTH);
		glutSolidCube(1);
	glPopMatrix();
}



void roof(void)
{
	// front left
	glPushMatrix();
		glColor3f(0, 1, 0);
		
		// rotate and translate into correct pos
		glTranslatef(-WALL_X-WIDTH_SPACE, -height/4.f, 0);
		glRotatef(-WALL_ROT_THETA, 0, -1, 0);

		// traslate to the pivot point of the shape
		glTranslatef(pointLocal[0], pointLocal[1] + height, pointLocal[2]); 
		glRotatef(-ROOF_ROT_THETA, 0, 0, 1);
		glTranslatef(-pointLocal[0], -pointLocal[1] - height, -pointLocal[2]); 
	
		glTranslatef(pointGlobal[0], height, 0);  
	
		roofTile();
	glPopMatrix();

	// front right
	glPushMatrix();
		glColor3f(1, 0, 0);
		
		// rotate and translate into correct pos
		glTranslatef(WALL_X+WIDTH_SPACE, -height/4.f, 0);
		glRotatef(WALL_ROT_THETA, 0, -1, 0);

		// traslate to the pivot point of the shape
		glTranslatef(pointLocal[0], pointLocal[1] + height, pointLocal[2]); 
		glRotatef(ROOF_ROT_THETA, 0, 0, 1);
		glTranslatef(-pointLocal[0], -pointLocal[1] - height, -pointLocal[2]); 
	
		glTranslatef(-pointGlobal[0], height, 0);  
	
		roofTile();
	glPopMatrix();

	// back left
	glPushMatrix();
		glColor3f(0, 1, 0);
		
		// rotate and translate into correct pos
		glTranslatef(-WALL_X-WIDTH_SPACE, -height/4.f, -WALL_Z);
		glRotatef(WALL_ROT_THETA, 0, -1, 0);

		// traslate to the pivot point of the shape
		glTranslatef(pointLocal[0], pointLocal[1] + height, pointLocal[2]); 
		glRotatef(-ROOF_ROT_THETA, 0, 0, 1);
		glTranslatef(-pointLocal[0], -pointLocal[1] - height, -pointLocal[2]); 
	
		glTranslatef(pointGlobal[0], height, 0);  
	
		roofTile();
	glPopMatrix();

	// back right
	glPushMatrix();
		glColor3f(0, 1, 0);
		
		// rotate and translate into correct pos
		glTranslatef(WALL_X+WIDTH_SPACE, -height/4.f, -WALL_Z);
		glRotatef(-WALL_ROT_THETA, 0, -1, 0);

		// traslate to the pivot point of the shape
		glTranslatef(pointLocal[0], pointLocal[1] + height, pointLocal[2]); 
		glRotatef(ROOF_ROT_THETA, 0, 0, 1);
		glTranslatef(-pointLocal[0], -pointLocal[1] - height, -pointLocal[2]); 
	
		glTranslatef(-pointGlobal[0], height, 0);  
	
		roofTile();
	glPopMatrix();
}

	float white[4] = {1., 1., 1., 1.};
	float black[4] = {0};

// displays the Muesums External structure 
void displayMuesum()
{
	walls();
	roof();
}


// ----------------------------------------------------------------------------
//  				  		Display OpenGL graphics				
// ----------------------------------------------------------------------------
void display(void)  
{
	const float lposHouse[4] = { 0.f, 3.f, -3.f, 1.f };  // light pos for house

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();

	// change camera
	if (viewState) {   // top down view
		glTranslatef(x_view_2, z_view_2, _zoom_);  // since transformed y has become z
		gluLookAt(0, 30, 0,  0, 0, -1, 0, 1, 0);
	} else {
		gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);	
	}


	// disable specular lighting ---------------------
	glMaterialfv(GL_FRONT, GL_SPECULAR, black); 


	// draw objects with only ambiant and diffuse lighting
	// displayMuesum();
		
	// skyBox();
	drawFloor();

	// reset back to white for any other objects that take light
  	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	// enable specular lighting ----------------------

	// Default scene lighting
	glLightfv(GL_LIGHT0, GL_POSITION, lposHouse);   // light for house

	// ball();
	boxCube();
	collBox(0);

	
	glutSwapBuffers();	

	showFPS();
}
