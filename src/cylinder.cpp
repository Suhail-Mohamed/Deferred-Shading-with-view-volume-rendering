//=============================================================================
// cylinder.h
//
//Author: Doron Nussbaum (C) 2023 All Rights Reserved.
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












#include "cylinder.h"


Cylinder::Cylinder()
{
}


Cylinder::~Cylinder()
{
}

/**************************************************************************************/
/* this function creates a cylinder.  The cylinder is genreated using two arrays.  The vertex data array which
contains the vertex data (geometry) and an index array which contains the topology of the triangles.  The trainagles
are stored in the index array as a triangle list.

Note that radius and height of the cylinder is 1

Input
numLong - number of longitudes lines.  For example if numLong == 10 than the sphere is divided into 10
of 36 degrees each


Ouptut:
vtx - an array (vector of vertices).  Currently the function computes the position and colour information. 

ind - an array (vector with the topology of the cylinder.


Return:
the function returns 0 is successful.


*/

int Cylinder::createCylinder(int numLong, Vertices &vtx, Indices &ind)

{
	int i, j, k;
	int numRows;
	int numCols;
	int numVtx;
	int numTriangles;
	Vector4f pos;
	Vector4f col;
	float alpha;
	float beta;
	float deltaAlpha;
	float deltaBeta;

	numRows = 1;  // number of horizonal slabs
	numCols = numLong;	// number of vertical slabs

	numVtx = (numRows + 1) * (numCols + 1);
	vtx.resize(numVtx);

	numTriangles = numRows * numCols * 2 + 2 * (numCols - 2);
	ind.resize(numTriangles * 3);

	// Fill the vertex buffer with positions
	k = 0;
	alpha = 0.0f;  // angle of latitude starting from the "south pole"
	deltaAlpha = (float)90.0; // increment of alpha
	beta = 0;   // angle of the longtidute 
	deltaBeta = (float)360.0 / (numLong);	// increment of beta

	for (i = 0, alpha = -45; i <= numRows; i++, alpha += deltaAlpha) {
		for (j = 0, beta = 0; j <= numCols; j++, beta += deltaBeta) {
			pos.x = cos(DegreeToRadians(alpha))*cos(DegreeToRadians(beta));
			pos.y = cos(DegreeToRadians(alpha))*sin(DegreeToRadians(beta));
			pos.z = sin(DegreeToRadians(alpha));
			pos.w = 1.0;

			vtx[k] = Vertex(pos, pos);
			k++;


		}
	}

	// fill the index buffer

	k = 0;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			// fill indices for the quad
			// change by making a quad function
			ind[k++] = i * (numCols + 1) + j;
			ind[k++] = i * (numCols + 1) + j + 1;
			ind[k++] = (i + 1) * (numCols + 1) + j + 1;

			ind[k++] = i * (numCols + 1) + j;
			ind[k++] = (i + 1) * (numCols + 1) + j + 1;
			ind[k++] = (i + 1) * (numCols + 1) + j;
		}
	}
	// fill the index buffer of the top plate	
	for (i = 0, j = numCols + 2; i < numCols - 2; i++, j++) {
		// fill indices of the top plate
		ind[k++] = numCols + 1;
		ind[k++] = j;
		ind[k++] = j + 1;
	}


	// fill the index buffer of the bottom plate	
	for (i = 0, j = 1; i < numCols - 2; i++, j++) {
		// fill indices of the top plate
		ind[k++] = 0;
		ind[k++] = j + 1;
		ind[k++] = j;
	}

	return(0);

}


