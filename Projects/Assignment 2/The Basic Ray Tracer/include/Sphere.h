//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:    Sphere.h
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
//  ==========================================================================


#ifndef SPHERE_H
#define SPHERE_H


#include <glm/glm.hpp>
#include "SceneObject.h"



// -----------------------------------------------------------------------------
//                 Defines a simple Sphere located at 'center'
//                          with the specified radius
// -----------------------------------------------------------------------------
class Sphere : public SceneObject 
{

	private:
		glm::vec3 center = glm::vec3(0);
		float radius = 1;

	public:
		Sphere() {};  //Default constructor creates a unit sphere

		Sphere(glm::vec3 c, float r) : center(c), radius(r) {}  // center & radius used in "Sphere.cpp"

		float intersect(glm::vec3 p0, glm::vec3 dir);

		glm::vec3 normal(glm::vec3 p);

};

#endif //!SPHERE_H
