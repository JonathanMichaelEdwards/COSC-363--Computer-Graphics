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
#define WALL_SCALE_HEIGHT   1.5
#define WALL_SCALE_LENGTH   5
#define FLOOR_BED           -1
#define WIDTH_SPACE         4

// move wall in place respect to angles
#define WALL_X     WALL_SCALE_LENGTH*sin(WALL_ROT_RAD)
#define WALL_Z     WALL_SCALE_LENGTH*cos(WALL_ROT_RAD)


const int doorHeight = WALL_SCALE_HEIGHT;
const float doorPointLocal[3] = {0.15, 0, 0};                     // pivot point
const float doorPointGlobal[3] = { };   // diff of point

#define DIST_Z     80
#define DIST_X     50
#define HEIGHT_Y   30

#define GL_CLAMP_TO_EDGE                        0x812F4




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
float look_x, look_z=-1, eye_x, eye_z;  //Camera parameters
GLdouble x_view=0,z_view=0, x_view_2=0,z_view_2=0, top_x=0, top_z=-1, top_x_2=0, top_z_2=-1, _zoom_=0;
  

// 3d person camera  ----------------------------------------
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


#define TEX 5
GLuint txId[TEX];   //Texture ids


// load textures - skybox
void _loadTGA(const char *_fileLoc, int index)
{
	char loc[100] = {0};

	glBindTexture(GL_TEXTURE_2D, txId[index]);

	sprintf(loc, "../Models/skybox4/%s.tga", _fileLoc);
	loadTGA(loc);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


// load the texture
//--------------------------------------------------------------------------------
void loadTexture()				
{
	glGenTextures(TEX, txId); 	// Create xxx texture ids

	_loadTGA("ft", 0);
	_loadTGA("bk", 1);
	_loadTGA("lt", 2);
	_loadTGA("rt", 3);
	_loadTGA("dn", 4);
	_loadTGA("up", 5);

	glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}


//--------------------------  sky box ------------------------------------------------------
void back(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[1]); // use floor ID 

	glBegin(GL_QUADS);
	glTexCoord2f(0,  1);       glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
	glTexCoord2f(0,  0);       glVertex3f(DIST_X, FLOOR_BED, DIST_Z);
	glTexCoord2f(1, 0);        glVertex3f(-DIST_X, FLOOR_BED, DIST_Z);
	glTexCoord2f(1, 1);        glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z); 
	
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void front(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[0]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);  
		glTexCoord2f(0,  0);       glVertex3f(-DIST_X, FLOOR_BED, -DIST_Z);  
		glTexCoord2f(1, 0);        glVertex3f(DIST_X, FLOOR_BED, -DIST_Z);  
		glTexCoord2f(1, 1);        glVertex3f(DIST_X, HEIGHT_Y, -DIST_Z); 
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void left(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[2]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);      glVertex3f(DIST_X, HEIGHT_Y, -DIST_Z);
		glTexCoord2f(0,  0);      glVertex3f(DIST_X, FLOOR_BED, -DIST_Z);
		glTexCoord2f(1, 0);       glVertex3f(DIST_X, FLOOR_BED, DIST_Z);
		glTexCoord2f(1, 1);       glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void right(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[3]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);      glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(0,  0);      glVertex3f(-DIST_X, FLOOR_BED, DIST_Z);
		glTexCoord2f(1, 0);       glVertex3f(-DIST_X, FLOOR_BED, -DIST_Z);
		glTexCoord2f(1, 1);       glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void floorBed(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[4]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(-DIST_X, FLOOR_BED, -DIST_Z);
		glTexCoord2f(0,  0);       glVertex3f(-DIST_X, FLOOR_BED, DIST_Z);
		glTexCoord2f(1, 0);        glVertex3f(DIST_X, FLOOR_BED, DIST_Z);
		glTexCoord2f(1, 1);        glVertex3f(DIST_X, FLOOR_BED, -DIST_Z);
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void top(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[5]); // use floor ID 

	glBegin(GL_QUADS);
		glTexCoord2f(0,  1);       glVertex3f(-DIST_X, HEIGHT_Y, DIST_Z);
		glTexCoord2f(0,  0);       glVertex3f(-DIST_X, HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 0);        glVertex3f(DIST_X, HEIGHT_Y, -DIST_Z);
		glTexCoord2f(1, 1);        glVertex3f(DIST_X, HEIGHT_Y, DIST_Z);
	glEnd();

	glDisable(GL_TEXTURE_2D);  // disable textures
}


void skyBox(void)
{	
	front();
	back();
	left();
	right();
	floorBed();
	top();
}
//--------------------------------------------------------------------------------



//// -------------- ball and ground collision -------------------------------
double da = 0.1;
double decr = 0.5;
bool resetBall = false;



#define PI		3.14159265358979323846
#define GRAVITY 9.81

#define velTheta      9
#define _velTheta     (velTheta * PI) / 180
#define airFric       1
#define t             1.2

double initSpeedX = 0;//20; 
double initSpeedY = 12;//16.1264;            // tan(theta) * Vx
double speedX = 0;
double speedY = 0;

bool _spacePressed = false;


bool spacePressed(bool _state)
{
	if (_state == _spacePressed) return false;  // button is ocupied
	_spacePressed = _state;
	return false;
}


void rstBall() 
{
	speedY = 0; 
	initSpeedY = 12; 
	da = 0.1;
	decr = 0.5;
	_spacePressed = false;
	resetBall = false;
}

double previous = 0;

void ball(void)
{
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(0, -0.8+speedX, 0);
		glutSolidSphere(0.1, 36, 18);
	glPopMatrix();
}


// collision between wall and ball - bad way
void ballBounce(int value) 
{  
	if (_spacePressed) {  
		previous = speedY;
		if (speedY > 1) { 
			speedY += (initSpeedY-=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t) - da;  // decrease fall height
		} else if (speedY < 1) {
			while (speedY < 1 - da) {  // makes sure it doesnt go past the origin
				resetBall = 0;
				da -= 0.1;
				speedY += (initSpeedY+=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);  // rise back to top from ground
			}
			da = (decr+=0.001);
	
			if (resetBall) {
				speedY += (initSpeedY+=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t); 
			 	if (abs((speedY-previous))  < 0.5) rstBall();
			}
			resetBall = true;
			
		}
	}

	speedX = speedY / 15;
	
	// glutPostRedisplay(); 
	glutTimerFunc(30, ballBounce, 0); 
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
	glScalef(WALL_SCALE_WIDTH, WALL_SCALE_HEIGHT, WALL_SCALE_LENGTH/2.5f);
	glutSolidCube(1);
}


void sideWall(void)
{
	glTranslatef(0, (WALL_SCALE_HEIGHT/2)-1, 0);        // y = -0.25  
	glScalef(WALL_SCALE_WIDTH, WALL_SCALE_HEIGHT, 3.7);
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
		glScalef(1, 1, 2.6);
		sideWall();
	glPopMatrix();
	

	// front left
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-WALL_X-WIDTH_SPACE+3, 0, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();
	
	// front left top
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(-WALL_X-WIDTH_SPACE+3, WALL_SCALE_HEIGHT, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();

	// front right
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(WALL_X+WIDTH_SPACE-3, 0, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();

	// front right top
	glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(WALL_X+WIDTH_SPACE-3, WALL_SCALE_HEIGHT, WALL_Z/2);   
		glRotatef(90, 0, 1, 0);
		sideWall();
	glPopMatrix();


	// front door - opens anti-clockwise
	glPushMatrix();
		glColor3f(0, 1, 0);

		glTranslatef(1.27, 0, 1.25 + 1.1); 
		glRotatef(150, 0, 1, 0);
		glTranslatef(-1.27, 0, -1.25 - 1.1); 
 
		glTranslatef(1.27, 0, 1.1); 

		openDoor();
	glPopMatrix(); 

	// front door top -- air con
	glPushMatrix();
		glColor3f(1, 1, 0);

		// glTranslatef(WALL_X+WIDTH_SPACE-5.85, 0, WALL_Z/2);  
		
		glTranslatef(1.27, WALL_SCALE_HEIGHT, 1.25 + 1.1); 
		glRotatef(90, 0, 1, 0);
		glTranslatef(-1.27, -WALL_SCALE_HEIGHT, -1.25 - 1.1); 
 
		glTranslatef(1.27, WALL_SCALE_HEIGHT, 1.1); 

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
#define ROOF_SCALE_HEIGHT   2
#define ROOF_SCALE_LENGTH   5

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
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();

	// change camera
	if (viewState) {   // top down view
		glTranslatef(x_view_2, z_view_2, _zoom_);  // since transformed y has become z
		gluLookAt(0, 30, 0,  0, 0, -1, 0, 1, 0);
	} else 
		gluLookAt(eye_x, 0, eye_z,  look_x, 0, look_z,   0, 1, 0);	

	displayMuesum();
	// eye = pos in surroundings
	// look is the rotation

	glDisable(GL_LIGHTING);	
	skyBox();
	glEnable(GL_LIGHTING);

	ball();
	
	glutSwapBuffers();	

	showFPS();
}
