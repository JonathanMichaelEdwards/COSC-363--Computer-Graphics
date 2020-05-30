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

vector<SceneObject*> sceneObjects;
TextureBMP texture;


//---The most important function in a ray tracer! ---------------------------------- 
//   Computes the colour value obtained by tracing a ray and finding its 
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
	glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
	glm::vec3 lightPos(10, 40, -3);					//Light's position
	glm::vec3 color(0);
	SceneObject *obj;
	static int patternCount = 0;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
	obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found

	// Chequered pattern
	rayChequeredFloor(obj, ray);
	rayWorldGlobe(obj, ray, texture);

	if (ray.index == 2)  {    
		if ((int)(ray.hit.x+ray.hit.y+ray.hit.z) % 2 == 0)  // By adding the hit.z - this adds a third degree of slope 
			obj->setColor(glm::vec3(0, patternCount/100.f, 0)); 
		else 
			obj->setColor(glm::vec3(1, 1, 0)); 

		if (++patternCount > 100) patternCount = 0;
	}

	

	color = obj->lighting(lightPos, -ray.dir, ray.hit);

	glm::vec3 lightVec = lightPos - ray.hit;
	Ray shadowRay(ray.hit, lightVec);

	shadowRay.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
	if((shadowRay.index > -1) && (shadowRay.dist < glm::length(lightVec))) color = 0.2f * obj->getColor();   // 0.2 = ambient scale factor

	if (obj->isReflective() && step < MAX_STEPS)  {   
		float rho = obj->getReflectionCoeff();   
		glm::vec3 normalVec = obj->normal(ray.hit);   
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);   
		Ray reflectedRay(ray.hit, reflectedDir);   
		glm::vec3 reflectedColor = trace(reflectedRay, step + 1);   
		color = color + (rho*reflectedColor);  
	}   


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

	// All scene obj's are created
	sceneShapes(sceneObjects, &texture);
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
