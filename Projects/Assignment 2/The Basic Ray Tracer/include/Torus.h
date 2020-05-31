#ifndef H_TORUS
#define H_TORUS
#include <glm/glm.hpp>
#include "SceneObject.h"


class Torus : public SceneObject
{

protected:
    glm::vec3 center = glm::vec3(0);
    float radius = 1;

public:	
	Torus() {}; //Default constructor creates a unit sphere
    Torus(glm::vec3 c, float r) : center(c), radius(r) {};

	float intersect(glm::vec3 p0, glm::vec3 dir);

	glm::vec3 normal(glm::vec3 p);

};

#endif //!H_TORUS
