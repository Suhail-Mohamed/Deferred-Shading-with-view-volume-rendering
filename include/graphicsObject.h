#pragma once
#ifndef GRPHICS_IOBJECT_HEADER
#define GRPHICS_IOBJECT_HEADER

//=============================================================================
// graphicsObject.h
//
//Author: Doron Nussbaum (C) 2015 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. generic class for graphics objects. 
//
//
// Description:
//--------------
//  sub classes of objects can be inherited.  The class does the main funcionality of setting and rendering objects.

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


#include <GL/glew.h>
#include "GL/glut.h"
#include "Shader.h"
#include "geometry_common.h"
#include "../include/nuss_vector.h"


/******************************************************************/
// STRUCTURES
typedef struct materials {
	Vector3f ambientMaterial;		// ambient material properties
	Vector3f diffuseMaterial;		// diffuse material properties
	Vector3f specularMaterial;		// specular material properties
	Vector3f interalRadiation;		// internal radiation of colour
} MATERIALS;




/*********************************************************************/

class GraphicsObject
{
public:
	GraphicsObject();
	virtual ~GraphicsObject();
	int createVAO(Shader shader);
	int createVAO(Shader shader, Vertices vtx, Indices ind);
	virtual int render(Shader shader);
	MATERIALS materials;
	virtual int loadMaterials(Shader shader);

protected:
	GLuint vtxVBO;  // the vertex buffer object which holds the vertices data
	GLuint vao;  // the vertex array object which holds all the data required for renderring.
	GLuint indVBO;	// index buffer for drawing the geometry
	float rollAngle;	// rotation around the z-axis
	float pitchAngle;	// rotation around the x-axis
	float yawAngle;		// rotation around the y-axis
	int numIndices;

public:
	void setInitialRotations(float rollAngle, float pitchAngle, float yawAngle);
	void setInitialPosition(Vector3f position);
	void setInitialPosition(float x, float y, float z);
	void incrementRotations(float rollAngle, float pitchAngle, float yawAngle);
	void incrementPosition(Vector3f position);
	void incrementPosition(float deltaX, float deltaY, float deltaZ);
	void setScale(Vector3f position);

	void setScale(float x, float y, float z);
	
	Vector3f scale;
	Vector3f position;
};

#endif
