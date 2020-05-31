//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:     RayTracer.cpp
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
// 
//
//  Description:   Implement a ray tracer that can handle different types of
//                 geometric objects and global illumination features, and 
//				   demonstrate its capability in enhancing the visual realism
//                 of a rendered scene.
//  ==========================================================================


#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include "Plane.h"
#include "TextureBMP.h"
#include "SceneShapes.h"
#include <GL/freeglut.h>
using namespace std;


#define          SAMPLES       4.f

const float WIDTH = 20.0;          // Width, height of the image plane in world units 
const float HEIGHT = 20.0;         

const float EDIST = 40.0;		// The distance of the image plane from the camera/origin. 
const int NUMDIV = 500;         // The number of cells (subdivisions of the image plane) along x and y directions.
const int MAX_STEPS = 5; 		// The number of levels (depth) of recursion (to be used next week) 

//  The boundary values of the image plane - Corner values
const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;


// Green Transparant sphere
#define       ETA             1.003f  // Snells law coef
#define       TRANSPARANT     0.4f


// Light sources
#define       LIGHT_SOURCE_1         glm::vec3(20, 40, -3)
#define       LIGHT_SOURCE_2         glm::vec3(-20, 30, -20)

// Light Source 1
#define       BRIGHT_LEVEL           0.3f       // 30 %
#define       AMBIENT                glm::vec3(0.2)


vector<SceneObject*> sceneObjects;
TextureBMP texture[3];



//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 color(0);
	SceneObject *obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found


	// -- Scene objects ray updates  ----------------------------------------------------------------
	// Chequered pattern
	rayChequeredFloor(obj, ray);

	rayWorldGlobe(obj, ray, texture[0]);
	raytable(obj, ray, texture[1]);
	rayTreasureMap(obj, ray, texture[2]);
	// ----------------------------------------------------------------------------------------------

	
	// -- Light Sources -------------------------------------------------------------
	// Source 2 - shadow right - Default main light - ambiant + diffuse + specular
	glm::vec3 lightVec2 = LIGHT_SOURCE_2 - ray.hit;  // light vec pos
	Ray rayShadow2(ray.hit, lightVec2);
	rayShadow2.closestPt(sceneObjects);				

	color += obj->lighting(LIGHT_SOURCE_2, -ray.dir, ray.hit);
	if((rayShadow2.index > -1) && (rayShadow2.dist < glm::length(lightVec2))) color = 0.2f * obj->getColor();   // 0.2 = ambient scale factor


	// Source 1 - shadow collide faint - ambiant + diffuse
	// Pg 5 of lec 7
	glm::vec3 currColor = sceneObjects[ray.index]->getColor();      // current index color
	glm::vec3 normVec1 = sceneObjects[ray.index]->normal(ray.hit);  // normalized vector
	glm::vec3 lightVec1 = LIGHT_SOURCE_1 - ray.hit;                 // light vec 

	float magLight = glm::length(lightVec1);                     // magnitude - dist
	lightVec1 = glm::normalize(lightVec1);                       // normalize light 
	float lDotn = glm::dot(lightVec1, normVec1);

	Ray rayShadow1(ray.hit, lightVec1);
	rayShadow1.closestPt(sceneObjects);
	
	// Id formula on pg 5
	if ((lDotn <= 0) || (rayShadow1.index > -1 && (rayShadow1.dist < magLight)))
		color += AMBIENT*(lDotn*currColor) + glm::vec3((BRIGHT_LEVEL/10.f))*(lDotn*currColor);  // Shadow overlap - make darker
	else
		color += AMBIENT*currColor + BRIGHT_LEVEL*(lDotn*currColor);                                  // default
	// ----------------------------------------------------------------------------------------------


	// -- Reflection of the Blue Sphere -------------------------------------------------------------
	if (obj->isReflective() && step < MAX_STEPS && ray.index == BLUE_SPHERE)  {   
		float rho = obj->getReflectionCoeff();   
		glm::vec3 normalVec = obj->normal(ray.hit);   
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);   

		Ray reflectedRay(ray.hit, reflectedDir);   
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);   
		color = color + (rho*reflectedColor);  
	}   
	// ----------------------------------------------------------------------------------------------


	// -- Transparency and Refraction -------------------------------------------------------------
	// -- pg 12 and 13 from lec 8
	if (step < MAX_STEPS && ray.index == TRANSPARANT_SPHERE) {
		// pg 19-22
		// 1. Find the first refraction vector
		// 2. Find the second refraction vector by the refracting the negative of the first normal
		// 3. Trace the color
		// Note: Refracted ray - use snells law

		// first vector
		glm::vec3 normalVec = obj->normal(ray.hit);   
		glm::vec3 dirFrac = glm::refract(ray.dir, normalVec, (1.0f/ETA));
		Ray rayFrac1(ray.hit, dirFrac);
		rayFrac1.closestPt(sceneObjects);

		// second vector
		normalVec = sceneObjects[rayFrac1.index]->normal(rayFrac1.hit); 
		dirFrac = glm::refract(dirFrac, -normalVec, ETA);
		Ray rayFrac2(rayFrac1.hit, dirFrac);
		rayFrac2.closestPt(sceneObjects);

		// trace
		glm::vec3 refracColor = trace(rayFrac2, step+1);
		color = TRANSPARANT * (color+refracColor);  
	}   
	// ----------------------------------------------------------------------------------------------


	return color;
}



// -----------------------------------------------------------------------------
//                            Anti-Aliasing Algorithm - Supersampling
//
//  - Generate several rays through each square pixel (eg. divide the pixel into
//    four equal segments) and compute the average of the colour values. 
//
//   - returns the average color value of 4 pixels from one pixel one - pg 34 lec 8
// -----------------------------------------------------------------------------
glm::vec3 antiAlising(Ray ray, glm::vec3 eye, float size, float xp, float yp)
{
	float quarter = size * (1/SAMPLES);
	float quarterHalf = size * (1 -(1.f/SAMPLES));
	
	glm::vec3 colorTot(0);
	
	// Divide ray into quaters - 1 beam in middle 
	ray = Ray(eye, glm::vec3(xp+quarter, yp+quarter, -EDIST));					
	colorTot += trace(ray, 1);
	
	// Divide quaters  - 2 beams 
	ray = Ray(eye, glm::vec3(xp+quarter, yp+quarterHalf, -EDIST));		
	colorTot += trace(ray, 1);
	ray = Ray(eye, glm::vec3(xp+quarterHalf, yp+quarter, -EDIST));		
	colorTot += trace(ray, 1);
	
	// divide half quater - 4 beams
	ray = Ray(eye, glm::vec3(xp+quarterHalf, yp+quarterHalf, -EDIST));			
	colorTot += trace(ray, 1);

	return colorTot /= SAMPLES;
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
			
			//Trace the primary ray and get the colour value - Anti-Alising algorithm used below
		    // glm::vec3 col = trace (ray, 1);
			glm::vec3 col = antiAlising(ray, eye, cellX, xp, yp);

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

	// All scene obj's are created
	sceneShapes(sceneObjects, texture);
}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracing");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}
