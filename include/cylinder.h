#pragma once

#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER
//=============================================================================
// cylinder.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a cylinder object. 
//
//
// Description:
//--------------
//  Creates a cylinder geometry.  Currently it is used only to create a geometry of a sphere. It can be converted to a stand alone function. 
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




#include "graphicsObject.h"
class Cylinder : public GraphicsObject
{
public:
	Cylinder();
	~Cylinder();
	static int createCylinder(int numLong, Vertices &vtx, Indices &ind);

};


#endif