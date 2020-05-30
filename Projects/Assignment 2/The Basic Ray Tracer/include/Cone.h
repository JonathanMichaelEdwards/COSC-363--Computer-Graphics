//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:    Cone.h
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
//  ==========================================================================


#ifndef CONE_H
#define CONE_H


#include <glm/glm.hpp>
#include "SceneObject.h"



// -----------------------------------------------------------------------------
//                 Defines a simple Cone located at 'center'
//                          with the specified radius
// -----------------------------------------------------------------------------
class Cone : public SceneObject
{
    private:
        glm::vec3 center = glm::vec3(0);
        float radius = 1;
        float height = 1;

    public:	
        Cone() {};  // Default constructor creates a unit Cone
        Cone(glm::vec3 c, float r, float h) : center(c), radius(r), height(h) {};

        float intersect(glm::vec3 pos, glm::vec3 dir);

        glm::vec3 normal(glm::vec3 p);
};


#endif //!CONE_H
