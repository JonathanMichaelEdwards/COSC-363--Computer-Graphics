//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:    Cone.cpp
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
//  ==========================================================================



#include "Cone.h"
#include <math.h>


#define SQUARE(DIR)                  DIR * DIR     // i.e a^2
#define DIR_POINT(H, POS, C)         (H - POS + C)
#define MAG(POS)                     (POS - center)



// -----------------------------------------------------------------------------
//              Cone's intersection method.  The input is a ray
// -----------------------------------------------------------------------------
float Cone::intersect(glm::vec3 pos, glm::vec3 dir)
{   
	// Intersection equation - shown on pg 45 (lec 8)
    // a^2x + bx + c = 0
    float a = SQUARE(dir.x) + SQUARE(dir.z) - (SQUARE(dir.y)*SQUARE((radius/height)));
    float b = 2*(MAG(pos).x*dir.x + MAG(pos).z*dir.z + (SQUARE((radius/height))*DIR_POINT(height, pos.y, center.y)*dir.y));
	float c = SQUARE(MAG(pos).x) + SQUARE(MAG(pos).z) - (SQUARE((radius/height))*DIR_POINT(height, pos.y, center.y));

    float delta = b*b - 4*(a*c);         // Discriminant part of the quadratic root
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
//                Assumption: The input point p lies on the Cone
// -----------------------------------------------------------------------------
glm::vec3 Cone::normal(glm::vec3 p)
{
	// Normal equation - pg 44 of lec 8
    float r = sqrt(SQUARE(MAG(p).x)+SQUARE(MAG(p).z));
    glm::vec3 n= glm::vec3 (MAG(p).x, r*(radius/height), MAG(p).z);
    return glm::normalize(n);

}
