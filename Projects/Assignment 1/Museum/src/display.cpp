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

#define FLOOR_X  26
#define FLOOR_Z  26

#define velTheta      90
#define _velTheta     (velTheta * PI) / 180
// #define airFric       0.47
#define t             0.01

double initSpeedX = 0; //20; 
double initSpeedY = 0; //16.1264;            // tan(theta) * Vx
double speedY = 0;
double speedX = 0;
double ballPosY = 3;  // pos height
double ballPosX = 0;  // pos width

// double u = ballPosY * sin(_velTheta);  // initial speed

bool _spacePressed = false;
double previous = 0;

bool accDown = true;
bool accUp = false;



#define DIST_Z     200
#define DIST_X     200
#define HEIGHT_Y   160

// #define GL_CLAMP_TO_EDGE                        0x812F




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






void box(void)
{
	glPushMatrix();
		// bottom left
		// glBegin(GL_TRIANGLES);
		// 	glColor3f(1, 0, 0);

		// 	glVertex3f(-ballPosX/2+0, ballPosY+0, 0);  
		// 	glVertex3f(-ballPosX/2+1, ballPosY+ 0, 0);  
		// 	glVertex3f(-ballPosX/2+0,  ballPosY+1, 0);  
		// glEnd();

		// // top right
		// glBegin(GL_TRIANGLES);
		// 	glColor3f(0, 1, 0);

		// 	glVertex3f(ballPosX/2+1, ballPosY+0, 0);  
		// 	glVertex3f(ballPosX/2+1, ballPosY+ 1, 0);  
		// 	glVertex3f(ballPosX/2+0,  ballPosY+1, 0); 
		// glEnd();

		// diagonal
		for (int i = 0; i < 3; i++) {
			glBegin(GL_TRIANGLES);
				glColor3f(0, 0, 1);
				// bottom
				glVertex3f(-ballPosX/2+0+i, ballPosY+0+i, 0);  
				glVertex3f(-ballPosX/2+1+i, ballPosY+ 0+i, 0);  
				glVertex3f(-ballPosX/2+0+i,  ballPosY+1+i, 0); 

				// top
				glColor3f(0, 1, 0);
				glVertex3f(ballPosX/2+1+i, ballPosY+0+i, 0);  
				glVertex3f(ballPosX/2+1+i, ballPosY+ 1+i, 0);  
				glVertex3f(ballPosX/2+0+i,  ballPosY+1+i, 0); 
			glEnd();
		}

	glPopMatrix();
}


#define BALL_RADIUS 0.1

void ball(void)
{
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(ballPosX, ballPosY, 0);
		glutSolidSphere(BALL_RADIUS, 36, 18);
	glPopMatrix();
}


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


#define DRAG_SPHERE 0.45 
#define AIR_DENSITY 1.225

bool reset = false;



void rstBall(void) 
{
	ballPosY = 3; 
	ballPosX = 0;
	speedY = 0;
	speedX = 0;
	_spacePressed = false;
	reset = false;
}


double da = 0.1;
double decr = 0.5;
bool resetBall = false;

void ballBounce(int value) 
{  
	double mass = 5.;
	double area = 4 * PI * (BALL_RADIUS*BALL_RADIUS);
	double vTerminal = sqrt((2*(mass)*GRAVITY) / (DRAG_SPHERE*AIR_DENSITY*area)) / 100; 

	if (_spacePressed) {  
		if (ballPosY+speedY >= FLOOR_BED+BALL_RADIUS) {
			reset = false;
			speedY -=  sin(_velTheta) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Gravity acceleration movement (drag)
			ballPosX += 0.1;  // x-direction movement
		} else {
			if (reset) rstBall();
			reset = true;
			speedY *= -1 + vTerminal;  // resistance percentage
		}
		ballPosY += speedY;
	}
	
	glutTimerFunc(10, ballBounce, 0); 
}




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

	ball();

	box();
	
	glutSwapBuffers();	

	showFPS();
}
