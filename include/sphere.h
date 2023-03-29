#pragma once

//=============================================================================
// sphere.h
//
//Author: Doron Nussbaum (C) 2023 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a sphere object. 
//
//
// Description:
//--------------
//  Creates sphere geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
//
//
// License
//--------------
//
// Code can be used for instructional and educational purposes.
// Usage of code for other purposes is not allowed without a given permission by the author.
//
//
// Disclaimer
//--------------
//
// The code is provided as is without any warranty

//=============================================================================



#ifndef SPHERE_HEADER
#define SPHERE_HEADER


#include "graphicsObject.h"
#include "geometry_common.h"

class Sphere : public GraphicsObject
{
public:
	Sphere();
	~Sphere();
	int createSphere(int numLong, int numLat, Vertices &vtx, Indices &ind);
	int createGeom(int numLongs, int numLat);
	int render(Shader shader);

	Vector4f sphere_colour;
};


#endif
