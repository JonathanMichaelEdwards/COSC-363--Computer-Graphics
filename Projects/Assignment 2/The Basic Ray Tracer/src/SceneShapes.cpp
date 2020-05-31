//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:     SceneShapes.cpp
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
// 
//
//  Description:  
//                Shapes/objects and light sources that are used in the scene.
//  ==========================================================================


#include "SceneShapes.h"
#include <GL/freeglut.h>



// -----------------------------------------------------------------------------
//                     Creates the base plane for the floor
// -----------------------------------------------------------------------------
void chequeredFloor(std::vector<SceneObject*> &sceneObjects)
{
	Plane *plane = new Plane 
	(
		glm::vec3(-50., -15, 50),    //Point A                            
		glm::vec3(50., -15, 50),     //Point B         
		glm::vec3(50., -15, -200),   //Point C                            
		glm::vec3(-50., -15, -200)   //Point D 
	);  

	plane->setSpecularity(false); 
	sceneObjects.push_back(plane); 
}


// -----------------------------------------------------------------------------
//              Creates a Chequered pattern for the base floor plane
// -----------------------------------------------------------------------------
void rayChequeredFloor(SceneObject *obj, Ray ray)
{
    glm::vec3 color(0);

	if (ray.index == FLOOR) {  
		int ix = (ray.hit.x+FLOOR_OFFSET) / FLOOR_QUAD_SIZE;   
		int iz = (ray.hit.z+FLOOR_OFFSET) / FLOOR_QUAD_SIZE;    
		
		if ((ix % 2 == 0 || iz % 2 == 0) && ix % 2 != iz % 2)  
			color = glm::vec3(1, 0, 0);
		else 
			color = glm::vec3(1, 1, 0);  
		obj->setColor(color);
	} 
}


// -----------------------------------------------------------------------------
//                     Creates the Chequered Floor in the scene
// -----------------------------------------------------------------------------
void table(std::vector<SceneObject*> &obj)
{
	Plane *top = new Plane 
	(
		glm::vec3(-10.5, -5, -50),    // Point A                            
		glm::vec3(10.5, -5, -50),     // Point B         
		glm::vec3(10.5, -5, -70),     // Point C                            
		glm::vec3(-10.5, -5, -70)     // Point D 
	);  

	Cylinder *stand =  new Cylinder
	(
		glm::vec3(0, -13, -65.0),  // Position
		2.0,                       // Radius
		8                          // Height
	);
	
	// top->setSpecularity(false); 
	obj.push_back(top); 

	// stand->setSpecularity(false);  	      // Remove specular light
	obj.push_back(stand);
}


void raytable(SceneObject *obj, Ray ray, TextureBMP texture)
{
	static int patternCount = 0;

	// Top texture
	if (ray.index == TABLE_TOP)  {    
		float x1 = -10.5;    float z1 = -50;
		float x2 = 10.5;     float z2 = -70;

		// Find coord pos - s and t
		float texS = (ray.hit.x-x1) / (x2-x1);
		float texT = (ray.hit.z-z1) / (z2-z1);

		obj->setColor(texture.getColorAt(texS, texT));
	}

	// Leg Procedual pattern
	if (ray.index == TABLE_LEG)  {    
		// By adding the hit.z - adds a second degree slope 
		if ((int)(ray.hit.x+ray.hit.y+ray.hit.z) % 2 == 0)
			obj->setColor(glm::vec3(0, patternCount/100.f, 0)); 
		else 
			obj->setColor(glm::vec3(1, 1, 0)); 

		if (++patternCount > 100) patternCount = 0;
	}
}


// -----------------------------------------------------------------------------
//                            Creates the world globe base
// -----------------------------------------------------------------------------
void worldGlobe(std::vector<SceneObject*> &obj)
{
	Sphere *globe = new Sphere
	(
		WORLD_GLOBE_POS,      // Position
		3.0                   // Radius
	); 

	Cone *cone = new Cone
	(
		CONE_STAND_GLOBE,     // Position
		2,                    // Radius
		5.f                   // Height
	);

	cone->setColor(glm::vec3(0, 0, 1));
	obj.push_back(cone);
	obj.push_back(globe);		           
}


// -----------------------------------------------------------------------------
//                       Maps Earth.bmp to the world globe base
//
// 	References:
//         https://people.cs.clemson.edu/~dhouse/courses/405/notes/texture-maps.pdf (pg 109)
//         https://www.mvps.org/directx/articles/spheremap.htm
// -----------------------------------------------------------------------------
void rayWorldGlobe(SceneObject *obj, Ray ray, TextureBMP texture)
{
	glm::vec3 color(0);

	if (ray.index == WORLD_GLOBE) { 
		glm::vec3 center = WORLD_GLOBE_POS;           // Centre of the sphere obj - to be mapped
		glm::vec3 d = glm::normalize(ray.hit-center);

		// Find coord pos - s and t
		float texS = (0.5-atan2(d.z, d.x)+M_PI) / (2*M_PI);  // X coord of the normal
		float texT = acos(-d.y) / M_PI;                      // Y coordinate of the normal

		color = texture.getColorAt(texS, texT);
		obj->setColor(color);
	}
}


// -----------------------------------------------------------------------------
//                           Draw a Tetrahedron
// -----------------------------------------------------------------------------
void drawTetrahedron(std::vector<SceneObject*> &obj)
{
	float x=2, y=-2, z=-60;
	int size = 3;

	// Points
	glm::vec3 A = glm::vec3(-size+x, -size+y, -size+z);
	glm::vec3 B = glm::vec3(size+x, -size+y, -size+z);
	glm::vec3 C = glm::vec3(0+x, size+y, 0+z);
	glm::vec3 D = glm::vec3(0+x, -size+y, size+z);
	
	// 4 different faces
	Plane *face1 = new Plane(A,B,C);
	Plane *face2 = new Plane(C,D,B);
	Plane *face3 = new Plane(B,D,A);
	Plane *face4 = new Plane(C,A,D);

	face1->setColor(glm::vec3(1, 0, 0));
	face2->setColor(glm::vec3(1, 0, 0));
	face3->setColor(glm::vec3(1, 0, 0));
	face4->setColor(glm::vec3(1, 0, 0));

	obj.push_back(face1);
	obj.push_back(face2);
	obj.push_back(face3);
	obj.push_back(face4);
}	


// -----------------------------------------------------------------------------
//             Creates the base for a blue sphere to be reflective
// -----------------------------------------------------------------------------
void reflectiveBlueSphere(std::vector<SceneObject*> &obj)
{
	Sphere *sphere = new Sphere
	(
		glm::vec3(10.5, 0.0, -90.0),   // Position
		8.0                            // Radius
	); 

	sphere->setColor(glm::vec3(0, 0, 1));   // Set colour to blue
	sphere->setReflectivity(true, 0.8);     // Set it to be reflective
	obj.push_back(sphere);      
}


// -----------------------------------------------------------------------------
//                           Draw a Torus -- Attempt
//   Reference:
//             https://marcin-chwedczuk.github.io/ray-tracing-torus
// -----------------------------------------------------------------------------
void drawTorus(std::vector<SceneObject*> &obj) 
{
// 	Torus *torus = new Torus(glm::vec3(0.0, 0.0, -50.0), 3.0);
// 	torus->setColor( glm::vec3(0, 0, 1));
// 	obj.push_back(torus);
}


// -----------------------------------------------------------------------------
//                     Creates the Background base plane
// -----------------------------------------------------------------------------
void drawBoard(std::vector<SceneObject*> &obj)
{
	Plane *back = new Plane 
	(
		glm::vec3(-30, -10, -150),   // Point A                            
		glm::vec3(30, -10, -150),    // Point B         
		glm::vec3(30, 20, -150),     // Point C                            
		glm::vec3(-30, 20, -150)     // Point D 
	);  

	back->setSpecularity(false);  	// remove specular light
	obj.push_back(back);
}


// -----------------------------------------------------------------------------
//                      Maps Treasure_Map.BMP to the background plane
//
// 	References:
//         http://www.aljanh.net/map-pirate-wallpapers/1436032319.html
// -----------------------------------------------------------------------------
void rayTreasureMap(SceneObject *obj, Ray ray, TextureBMP texture)
{
	if (ray.index == TREASURE_MAP)  {    
		float x1 = -30;    float y1 = -10;
		float x2 = 30;     float y2 = 20;

		// Find coord pos - s and t
		float texS = (ray.hit.x-x1) / (x2-x1);
		float texT = (ray.hit.y-y1) / (y2-y1);

		obj->setColor(texture.getColorAt(texS, texT));
	}
}


// -----------------------------------------------------------------------------
//             Creates the base for a blue sphere to be reflective
// -----------------------------------------------------------------------------
void transparantSphere(std::vector<SceneObject*> &obj)
{
	Sphere *sphere = new Sphere
	(
		glm::vec3(0.5, 2.0, -45.0),   // Position
		2.0                           // Radius
	); 

	sphere->setColor(glm::vec3(0, 1, 0));   // Set colour to green
	sphere->setSpecularity(false);  	    // remove specular light
	obj.push_back(sphere);      
}


// -----------------------------------------------------------------------------
//                           Shapes drawn in the scene
// -----------------------------------------------------------------------------
void sceneShapes(std::vector<SceneObject*> &sceneObjects, TextureBMP *texture) 
{
    chequeredFloor(sceneObjects);        // 1 index's
    table(sceneObjects);                 // 2 index's

	worldGlobe(sceneObjects);            // 2 index's
	drawTetrahedron(sceneObjects);       // 4 index's
	reflectiveBlueSphere(sceneObjects);  // 1 index's
	// drawTorus(sceneObjects);       
	drawBoard(sceneObjects);             // 1 index's
	transparantSphere(sceneObjects);     // 1 index's

	texture[0] = TextureBMP("../Models/Earth.bmp");          // http://www.world-maps.org/
	texture[1] = TextureBMP("../Models/Table.bmp");          // https://freestocktextures.com/texture/floor-wood-oak,765.html
	texture[2] = TextureBMP("../Models/Treasure_Map.bmp");	 // http://www.aljanh.net/map-pirate-wallpapers/1436032319.html
}
