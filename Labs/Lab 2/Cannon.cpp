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

#define velTheta      38.88 
#define _velTheta     (velTheta * PI) / 180
#define airFric       0.35
#define t             1

double initSpeedX = 20; 
double initSpeedY = 16.1264;            // tan(theta) * Vx
double speedX = 0;
double speedY = 0;

bool spacePressed = false;





//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)  
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------
void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}

//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{
	glColor3f(0.4, 0.5, 0.4);

    //Construct the object model here using triangles read from OFF file
	glBegin(GL_TRIANGLES);
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
	glEnd();
}

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


// Create mount bracket object for cannon.
void mntBrack(GLfloat xT, GLfloat yT, GLfloat zT, GLfloat xS, GLfloat yS, GLfloat zS)
{
	glPushMatrix();
		glColor3f(1, 0.8, 0);
		glTranslatef(xT, yT, zT); 
		glScalef(xS, yS, zS);
		glutSolidCube(1);
	glPopMatrix();
}


// Create and fire a cannon ball as a projectile.
void cannonBall(GLfloat x, GLfloat y, GLfloat z)
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
	gluLookAt(0, cam_hgt, 200, 0, 0, 0, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);   //set light position

    glRotatef(angle, 0.0, 1.0, 0.0);		//rotate the whole scene

	drawFloor();





	// Transformation = changes the vertex/vector coordinates 

	// rotate pivot point then draw cannon
	glPushMatrix();
		glTranslatef(-20, 30, 0);    // Pivot point coordinates    3. Cancel first Translation to get original Translation made
		glRotatef(30, 0, 0, 1);      // Rotate pivot points        2. Rotate object
		glTranslatef(20, -30, 0);    // inverse of pivot points    1. Translate object to position
		drawCannon();                // draw object                0. Draw object
	glPopMatrix();

	// create brackets
	mntBrack(-10, 5, 17, 80, 10, 6);   // first cube
	mntBrack(-20, 25, 17, 40, 30, 6);  // second cube

	// mirrored bracket
	mntBrack(-10, 5, -17, 80, 10, 6); 
	mntBrack(-20, 25, -17, 40, 30, 6);
	
	// To do next: project a cannon ball as it is fired from the cannon
	cannonBall(0,0,0);





	glFlush();
}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    loadMeshFile("Cannon.off");				//Specify mesh file name here
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


void resetCannon() 
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
		if (speedY > -64) {
			speedX += initSpeedX * t * cos(_velTheta);                                  // Speed X stays constant
			speedY += (initSpeedY-=airFric) * t * sin(_velTheta) - 0.5 * GRAVITY * (t*t);   // Speed Y changes due to gravity and air friction
		} else resetCannon();
	}

	glutPostRedisplay(); 
	glutTimerFunc(30, myTimer, 0); 
}




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
