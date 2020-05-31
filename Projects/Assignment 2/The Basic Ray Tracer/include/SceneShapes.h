//  ==========================================================================
//  COSC 363: Computer Graphics (2020);  University of Canterbury
//  Project:  The Basic Ray Tracer 
//
//  FILE NAME:     SceneShapes.h
//  
//  Author:       Jonathan Edwards
//  Co-Author:    Department of Computer Science and Software Engineering
//  ==========================================================================


#ifndef SCENE_SHAPES_H
#define SCENE_SHAPES_H


#include <iostream>
#include <vector>
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Torus.h"
#include "Ray.h"
#include "TextureBMP.h"


// Constants
#define M_PI		3.14159265358979323846

// Texture indexes
#define    FLOOR                  0
#define    TABLE_TOP              1
#define    TABLE_LEG              2
#define    WORLD_GLOBE            4
#define    BLUE_SPHERE            9
#define    TREASURE_MAP           10
#define    TRANSPARANT_SPHERE     11

// Colors and positions
#define    TABLE_TOP_POS_C     glm::vec3(0, -5, -60)  
#define    WORLD_GLOBE_POS     glm::vec3(-6.5, 3, -60.0)
#define    CONE_STAND_GLOBE    glm::vec3(-6.5, -5.0, -60.0)


  
#define    FLOOR_OFFSET         50
#define    FLOOR_QUAD_SIZE      5



void rayChequeredFloor(SceneObject *obj, Ray ray);
void rayWorldGlobe(SceneObject *obj, Ray ray, TextureBMP texture);
void raytable(SceneObject *obj, Ray ray, TextureBMP texture);
void rayTreasureMap(SceneObject *obj, Ray ray, TextureBMP texture);

void sceneShapes(std::vector<SceneObject*> &sceneObjs, TextureBMP *texture);


#endif // !SCENE_SHAPES_H
