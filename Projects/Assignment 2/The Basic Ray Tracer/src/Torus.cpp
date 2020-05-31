#include "Torus.h"
#include <math.h>


#define SQUARE(DIR)                  (DIR.x*DIR.x) + (DIR.y*DIR.y) + (DIR.z*DIR.z)  
#define DIR_POINT(DIR, POS, C)       DIR.x*(POS.x-C.x) + DIR.y*(POS.y-C.y) + DIR.z*(POS.z-C.z)



float Torus::intersect(glm::vec3 pos, glm::vec3 dir)
{
    // Intersection equation 
    // a^4x + bx^3 + cx^2 + dx +e = 0
    // float a = 
    // float b = 
    // float c = 
    // float e = 
    
    // float delta = b*b - 4*(a*c);           // Discriminant part of the quadratic root
	// if(fabs(delta) < 0.001) return -1.0; 
    // if(delta < 0.0) return -1.0;
    
    // // Quadratic Formula - to find the roots
    // float t1 = (-b - sqrt(delta)) / (2*a);
    // float t2 = (-b + sqrt(delta)) / (2*a);
    // float y, inter;
    
    // // // Find intersection point
    // if (t1 > t2) inter = t2;
    // else inter = t1;
    
    // // point of inter
    // y = pos.y + inter*dir.y;
    
    // if ((y >= center.y) && (y <= center.y)) return inter;
    // else return -1;
    
    return -1;
}


glm::vec3 Torus::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3 (p.x-center.x,0,p.z-center.z);
    return glm::normalize(n);
}
