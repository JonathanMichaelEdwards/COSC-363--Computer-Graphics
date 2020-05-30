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

	if (ray.index == FLOOR)   {  
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
		glm::vec3(-8., -5, -40),    // Point A                            
		glm::vec3(8., -5, -40),     // Point B         
		glm::vec3(8., -5, -60),     // Point C                            
		glm::vec3(-8., -5, -60)     // Point D 
	);  

	Cylinder *stand =  new Cylinder
	(
		glm::vec3(0, -20.0, -60.0),  // Position
		2.0,                         // Radius
		15                           // Height
	);
	
	top->setColor(glm::vec3(0, 1, 0));
	top->setSpecularity(false); 
	obj.push_back(top); 

	stand->setColor(glm::vec3(0, 0, 1));  // Set colour to blue
	stand->setSpecularity(false);  	     // Remove specular light
	obj.push_back(stand);
}


// -----------------------------------------------------------------------------
//                             Creats the world globe
// -----------------------------------------------------------------------------
void worldGlobe(std::vector<SceneObject*> &obj)
{
	Plane *top = new Plane 
	(
		glm::vec3(-8., -5, -40),    // Point A                            
		glm::vec3(8., -5, -40),     // Point B         
		glm::vec3(8., -5, -60),     // Point C                            
		glm::vec3(-8., -5, -60)     // Point D 
	);  

	Cylinder *stand =  new Cylinder
	(
		glm::vec3(0, -20.0, -60.0),  // Position
		2.0,                         // Radius
		15                           // Height
	);
	
	top->setColor(glm::vec3(0, 1, 0));
	top->setSpecularity(false); 
	obj.push_back(top); 

	stand->setColor(glm::vec3(0, 0, 1));  // Set colour to blue
	stand->setSpecularity(false);  	     // Remove specular light
	obj.push_back(stand);
}


// -----------------------------------------------------------------------------
//                           Shapes drawn in the scene
// -----------------------------------------------------------------------------
void sceneShapes(std::vector<SceneObject*> &sceneObjects, TextureBMP *texture) 
{
    chequeredFloor(sceneObjects);
    table(sceneObjects);

	worldGlobe(sceneObjects);

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

	*texture = TextureBMP("../Models/Butterfly.bmp");
}