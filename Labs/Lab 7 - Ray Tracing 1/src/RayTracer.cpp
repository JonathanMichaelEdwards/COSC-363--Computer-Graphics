/*==================================================================================
* COSC 363  Computer Graphics (2020)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab07.pdf, Lab08.pdf for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
using namespace std;

const float WIDTH = 20.0;          // Width, height of the image plane in world units 
const float HEIGHT = 20.0;         

const float EDIST = 40.0;		// The distance of the image plane from the camera/origin. 
const int NUMDIV = 500;          // The number of cells (subdivisions of the image plane) along x and y directions.
const int MAX_STEPS = 5; 		// The number of levels (depth) of recursion (to be used next week) 

//  The boundary values of the image plane - Corner values
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	// -- removed --
	// color = obj->getColor();						//Object's colour

	// step 2-3
	// note: -- Look on pg 8 of lec 8 for diagram
	//     - direction of the view vector = -ray.dir
	//     - point of intersection        =  ray.hit
	color = obj->lighting(lightPos, -ray.dir, ray.hit);

	// step 4
	// direction of light source vector - i.e if dir = <-1, 3, -6> take abs dir = <1, 3, 6>
	glm::vec3 lightVec = lightPos - ray.hit;
	// glm::vec3 lightVec(0);   
	// float magRay = 0.f;

	// - find abs dir, i.e sqrt((b-a) * (b-a)) (abs dir between two points)     - dir = Vector
	// - find magnitude of abs dir, i.e sum of abs dir (x+y+z) - length         - mag = Scalar
	// for (int i = 0; i < 3; i++) {
	// 	magRay += (lightVec[i] = sqrt((ray.hit[i]-lightPos[i]) * (ray.hit[i]-lightPos[i])));  
	// }

	Ray shadowRay(ray.hit, lightVec);

	shadowRay.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    // if((shadowRay.index > -1) && (shadowRay.dist < magRay)) color = 0.2f * obj->getColor();   // 0.2 = ambient scale factor
	
	if((shadowRay.index > -1) && (shadowRay.dist < glm::length(lightVec))) color = 0.2f * obj->getColor();   // 0.2 = ambient scale factor
	
	// glm::length(lightVec) (len=sqrt(x * x)) is equivalent to 
	// glm::distance(lightPos, ray.hit) (len=sqrt((b-a) * (b-a)))

	return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
	float xp, yp;  //grid point
	float cellX = (XMAX-XMIN)/NUMDIV;  //cell width
	float cellY = (YMAX-YMIN)/NUMDIV;  //cell height
	glm::vec3 eye(0., 0., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);  //Each cell is a tiny quad.

	for(int i = 0; i < NUMDIV; i++)	//Scan every cell of the image plane
	{
		xp = XMIN + i*cellX;
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;

			// Centre value of a single cell
		    glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray

		    Ray ray = Ray(eye, dir);

		    glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value

			glColor3f(col.r, col.g, col.b);
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}



//---This function initializes the scene ------------------------------------------- 
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{		
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

	// look at fig 7 for more details
	// #note: (z < -EDIST)
	// Sphere 1 - blue
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 0.0, -90.0), 15.0);  // Use the sphere object
	sphere1->setColor(glm::vec3(0, 0, 1));   //Set colour to blue
	// sphere1->setSpecularity(false);  	-- remove specular light
	// sphere1->setShininess(5);        	-- change shininess (small val = stronger)
	sceneObjects.push_back(sphere1);		 //Add sphere to scene objects

	// Sphere 2 - green
	Sphere *sphere2 = new Sphere(glm::vec3(5.0, -10.0, -60.0), 5.0);  // Use the sphere object
	sphere2->setColor(glm::vec3(0, 1, 0));   //Set colour to green
	sceneObjects.push_back(sphere2);		 //Add sphere to scene objects

	// Sphere 3 - red
	Sphere *sphere3 = new Sphere(glm::vec3(5.0, 5.0, -70.0), 4.0);  // Use the sphere object
	sphere3->setColor(glm::vec3(1, 0, 0));   //Set colour to red
	sceneObjects.push_back(sphere3);		 //Add sphere to scene objects

	// Sphere 4 - turquoise
	Sphere *sphere4 = new Sphere(glm::vec3(10.0, 10.0, -60.0), 3.0);  // Use the sphere object
	sphere4->setColor(glm::vec3(64.f/255.f, 244.f/255.f, 208.f/255.f));   //Set colour to turquoise
	sceneObjects.push_back(sphere4);		 //Add sphere to scene objects
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
