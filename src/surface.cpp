
//=============================================================================
// Surface.c
//
//Author: Doron Nussbaum (C) 2023 All Rights Reserved.
//-----------------------------------------------------
//
// Purpose: 
//--------------
// a. creates a Surface object. 
//
//
// Description:
//--------------
//  Creates Surface geometry.  Currently it is used only to create a geometry of a Surface. It can be converted to a stand alone function. 
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



/*********************************************************************************/
// INCLUDE FILES

#include "surface.h"
#include <iostream>
using namespace std;


/*********************************************************************************/
// DEFINES
#define MIN_X_RANGE -1.0
#define MAX_X_RANGE 1.0

#define MIN_Z_RANGE -1.0
#define MAX_Z_RANGE 1.0

/*********************************************************************************/


/*********************************************************************************/

Surface::Surface()
{
}


Surface::~Surface()
{
}






/*
// Function creates a Surface geometry

input:
numRows - number of rows in the surface
numCols - number of columns in the surface


*/

int  Surface::createSurface(int numRows, int numCols, Vertices &vtx, Indices &ind)
{
	int i, j, k;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	Vector4f colour = Vector4f(0,0,0,1);
	float x;
	float z;
	float deltaX;
	float deltaZ;


	numVtx = (numRows + 1) * (numCols + 1);		// include the addtional vertices along the boundary of the surface
	//vtx.resize(numVtx);
	vtx.resize(0);
	numTriangles = numRows * numCols * 2;
	
	//ind.resize(numTriangles * 3);
	ind.resize(0);

	// Fill the vertex buffer with positions
	k = 0;
	deltaX = (float)(MAX_X_RANGE - MIN_X_RANGE) / numCols; // increment along the x-axis
	deltaZ = (float)(MAX_Z_RANGE - MIN_Z_RANGE) / numRows; // increment along the y-axis


	for (i = 0, z = MIN_Z_RANGE; i <= numRows; i++, z += deltaZ) {
		for (j = 0, x = MIN_X_RANGE; j <= numCols; j++, x += deltaX) {
			pos.x = x;
			pos.y = 0;
			pos.z = z;
			pos.w = 1.0;
			
			// add colour 
			colour.x = j * deltaX / 2.0f;
			colour.z = i * deltaZ / 2.0f;
			//colour = Vector4f(1, 0, 0, 1);

			vtx.push_back(Vertex(pos, colour));
			k++;


		}
	}

	// fill the index buffer
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			createQuad(numRows, numCols, i, j, ind);
		}
	}

	return(0);

}

/*******************************************************************************/
// creates two triangoles indices of quad at position row, col

int Surface::createQuad(int NumRows, int numCols, int row, int col, Indices &ind)
{
	// fill indices for the quad
	// change by making a quad function
	int i, j, k;
	i = row * (numCols + 1) + col;
	j = (row + 1) * (numCols + 1) + col + 1;
	k = row * (numCols + 1) + col + 1;


	ind.push_back(row * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col + 1);
	ind.push_back(row * (numCols + 1) + col + 1);

	i = (row * (numCols + 1) + col);
	j = (row + 1) * (numCols + 1) + col;
	k = (row + 1) * (numCols + 1) + col + 1;

	ind.push_back(row * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col);
	ind.push_back((row + 1) * (numCols + 1) + col + 1);


	return 0;
}





/*********************************************************************************/


int Surface::render(Shader shader)
{
	Matrix4f rotMat;  // rotation matrix;
	Matrix4f scaleMat; // scaling matrix;
	Matrix4f transMat;	// translation matrix
	Matrix4f modelMat;	// final model matrix

	// set the transformation matrix - the model transfomration
	modelMat = Matrix4f::identity(); // = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);

	// set the scaling - this is model space to model space transformation
	scaleMat = Matrix4f::scale(scale.x, scale.y, scale.z);
	modelMat = scaleMat * modelMat;

	// set the rotation  - this is model space to model space transformation 
	rotMat = Matrix4f::rotateRollPitchYaw(rollAngle, pitchAngle, yawAngle, 1);
	// note that we always multiply the new matrix on the left
	modelMat = rotMat * modelMat;

	// set the orientation  - this is model space to model space transformation 
	// Add code here


	// set the translation - this is model space to world space transformation
	transMat = Matrix4f::translation(position);
	modelMat = transMat * modelMat;

	// move matrix to shader
	shader.copyMatrixToShader(modelMat, "model");


	// bind the vao
	glBindVertexArray(vao);

	// draw using indices
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, NULL);

	// unbind the vao
	glBindVertexArray(0);

	return 0;
}





