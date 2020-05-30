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
		glm::vec3(0, -14, -70.0),  // Position
		2.0,                       // Radius
		8                          // Height
	);
	
	// top->setColor(glm::vec3(0, 1, 0));
	top->setSpecularity(false); 
	obj.push_back(top); 

	stand->setColor(glm::vec3(0, 0, 1));  // Set colour to blue
	stand->setSpecularity(false);  	      // Remove specular light
	obj.push_back(stand);
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


//----------------------Draw tetrahedron---------------------------------------------
void drawTetrahedron(std::vector<SceneObject*> &obj)
{
	float x=0, y=-1, z=-24;
	
	// Points
	glm::vec3 A = glm::vec3(-1+x, -1+y, -1+z);
	glm::vec3 B = glm::vec3(1+x, -1+y, -1+z);
	glm::vec3 C = glm::vec3(0+x, 1+y, 0+z);
	glm::vec3 D = glm::vec3(0+x, -1+y, 1+z);
	
	Plane *face1 = new Plane(A,B,C);
	Plane *face2 = new Plane(C,D,B);
	Plane *face3 = new Plane(B,D,A);
	Plane *face4 = new Plane(C,A,D);

	obj.push_back(face1);
	obj.push_back(face2);
	obj.push_back(face3);
	obj.push_back(face4);
}	


// -----------------------------------------------------------------------------
//                           Shapes drawn in the scene
// -----------------------------------------------------------------------------
void sceneShapes(std::vector<SceneObject*> &sceneObjects, TextureBMP *texture) 
{
    chequeredFloor(sceneObjects);
    table(sceneObjects);

	worldGlobe(sceneObjects);
	drawTetrahedron(sceneObjects);




	// Cylinder *sphere1 =  new Cylinder(glm::vec3(-5.0, 0.0, -90.0), 15.0, 1);  // Use the sphere object
	// sphere1->setColor(glm::vec3(0, 0, 1));   //Set colour to blue
	// // sphere1->setSpecularity(false);  	-- remove specular light
	// // sphere1->setShininess(5);        	-- change shininess (small val = stronger)
	// sphere1->setReflectivity(true, 0.8); 
	// sceneObjects.push_back(sphere1);		 //Add sphere to scene objects

	// // Sphere 2 - green
	// Sphere *sphere2 = new Sphere(glm::vec3(5.0, -10.0, -60.0), 5.0);  // Use the sphere object
	// sphere2->setColor(glm::vec3(0, 1, 0));   //Set colour to green
	// sceneObjects.push_back(sphere2);		 //Add sphere to scene objects

	// // Sphere 3 - red
	// Sphere *sphere3 = new Sphere(glm::vec3(5.0, 5.0, -70.0), 4.0);  // Use the sphere object
	// sphere3->setColor(glm::vec3(1, 0, 0));   //Set colour to red
	// sceneObjects.push_back(sphere3);		 //Add sphere to scene objects

	// // Sphere 4 - turquoise
	// Sphere *sphere4 = new Sphere(glm::vec3(10.0, 10.0, -60.0), 3.0);  // Use the sphere object
	// sphere4->setColor(glm::vec3(64.f/255.f, 244.f/255.f, 208.f/255.f));   //Set colour to turquoise
	// sceneObjects.push_back(sphere4);		 //Add sphere to scene objects

    // Plane *plane_B_6 = new Plane(Vector(-20,-20,-150),Vector(20,-20,-150),Vector(20,20,-150),Vector(-20,20,-150), Color::GRAY);
    // sceneObjects.push_back(plane_B_6);//background
    // Plane *plane_L_7 = new Plane(Vector(-20,-20,-30),Vector(-20,-20,-150), Vector(-20, 20,-150),Vector(-20, 20,-30), Color(1,1,1));
    // sceneObjects.push_back(plane_L_7);
    // Plane *plane_R_8 = new Plane(Vector(20,20,-30),Vector(20,20,-150), Vector(20, -20,-150),Vector(20, -20,-30), Color(1,1,1));
    // sceneObjects.push_back(plane_R_8);
    // Plane *plane9 = new Plane(Vector(-20,20,-30),Vector(20,20,-30),Vector(20,20,-150),Vector(-20,20,-150), Color(1,1,1));
    // sceneObjects.push_back(plane9);

	*texture = TextureBMP("../Models/Earth.bmp");
}