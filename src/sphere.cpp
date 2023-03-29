
//=============================================================================
// sphere.c
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


#include "sphere.h"
#include <iostream>
using namespace std;



Sphere::Sphere()
{
}


Sphere::~Sphere()
{
	GraphicsObject::~GraphicsObject();
}






/*
// Function creates a sphere geometry

input:
numLong - number of longitudes
numLat - number of latituudes.  Note that the number of latitudes is defined only for the northern hemishphere


*/

int Sphere::createSphere(int numLong, int numLat, Vertices &vtx, Indices &ind)
{
	int i, j, k;
	int numRows;
	int numCols;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	float alpha;
	float beta;
	float deltaAlpha;
	float deltaBeta;

	numRows = numLat * 2;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

	numVtx = (numRows + 1) * (numCols + 1);
	vtx.resize(0);
	
	numTriangles = numRows * numCols * 2;
	ind.resize(0);

	// Fill the vertex buffer with positions
	k = 0;
	alpha = 0.0f;  // angle of latitude starting from the "south pole"
	deltaAlpha = (float)90.0 / numLat; // increment of alpha
	beta = 0;   // angle of the longtidute 
	deltaBeta = (float)360.0 / (numLong);	// increment of beta

	for (i = 0, alpha = -90; i <= numRows; i++, alpha += deltaAlpha) {
		for (j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			pos.x = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));
			pos.y = sin(DegreeToRadians(alpha));
			pos.z = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta));
			pos.w = 1.0;

			vtx.push_back(Vertex(pos, Vector3f(pos.x, pos.y, pos.z), sphere_colour));
		}
	}

	// fill the index buffer
	k = 0;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			// change by making a quad function
			ind.push_back(i * (numCols + 1) + j);
			ind.push_back(i * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);

			ind.push_back(i * (numCols + 1) + j);
			ind.push_back((i + 1) * (numCols + 1) + j + 1);
			ind.push_back((i + 1) * (numCols + 1) + j);
		}
	}


	return(0);

}

/*********************************************************************************/

int Sphere::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	modelMat = Matrix4f::identity(); 

	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	modelMat = rotMat * modelMat;

	rotMat = Matrix4f::rotateRollPitchYaw(0, 0, 0, 1);
	modelMat = rotMat * modelMat;

	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;

	shader.copyMatrixToShader(modelMat, "model");
	
	glBindVertexArray(vao);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);
	
	glBindVertexArray(0);
	return 0;
}

/*******************************************************************************/

int Sphere::createGeom(int numLongs, int numLat)
{
//	int numLong, int numLat, float radius, struct sphereVertex **vtx, int *numVtx1, GLuint **ind, int *numInd1
	return 0;
}


