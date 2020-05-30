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
#include "Ray.h"
#include "TextureBMP.h"

#define    FLOOR            0
#define    TABLE_TOP        1

#define    FLOOR_OFFSET         50
#define    FLOOR_QUAD_SIZE      5



void rayChequeredFloor(SceneObject *obj, Ray ray);
void sceneShapes(std::vector<SceneObject*> &sceneObjs, TextureBMP *texture);


#endif // !SCENE_SHAPES_H
