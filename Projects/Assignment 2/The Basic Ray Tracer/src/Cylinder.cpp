//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:    Cylinder.cpp
//  
//  Author:       Jonathan Edwards
//  ==========================================================================


#include "Cylinder.h"
#include <math.h>


#define SQUARE(DIR)    DIR * DIR  // i.e a^2
#define ROOT_A         SQUARE(dir.x) + SQUARE(dir.z);

#define DIR_POINT(DIR, POS, C)       DIR*(POS-C)
#define ROOT_B                      (DIR_POINT(dir.x, pos.x, center.x) + DIR_POINT(dir.z, pos.z, center.z))



// -----------------------------------------------------------------------------
//              Cylinder's intersection method.  The input is a ray
// -----------------------------------------------------------------------------
float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir)
{   
    // Intersection equation - shown on pg 38 (lec 8)
    // a^2x + bx + c = 0
    float a = ROOT_A;
    float b = 2 * ROOT_B;
    float c = SQUARE((pos.x-center.x)) + SQUARE((pos.z - center.z)) - (radius*radius);
    
    float delta = b*b - 4*(a*c);           // Discriminant part of the quadratic root
	if(fabs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;
    
    // Quadratic Formula - to find the roots
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    float y, inter;
    
    // Find intersection point
    if (t1 > t2) inter = t2;
    else inter = t1;
    
    // point of inter
    y = pos.y + inter*dir.y;
    
    if ((y >= center.y) && (y <= center.y + height)) return inter;
    else return -1;
}


// -----------------------------------------------------------------------------
//                Returns the unit normal vector at a given point
//                Assumption: The input point p lies on the Cylinder
// -----------------------------------------------------------------------------
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3 (p.x-center.x,0,p.z-center.z);
    return glm::normalize(n);
}
