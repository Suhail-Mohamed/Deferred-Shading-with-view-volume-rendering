#pragma once

#ifndef LIGHT_HEADER
#define LIGHT_HEADER

/***************************************************************************/
//	INCLUDES

#include "../include/nuss_vector.h"
#include "Shader.h"
#include "sphere.h"

/***************************************************************************/

typedef struct pointLight {
	Vector3f lightIntensity;
	Vector3f ambientIntensity;	
	Vector3f worldPos;			
	float    specularPower;	
} PointLight;

typedef struct attenuation {
	float a;
	float b;
	float c;
} Attenuation;


class Light
{
public:
	Light();
	~Light();
	int loadPointLight(Shader shader);
	void set_attenuation(int a,int b, int c) {
		point_light_attenuation.a = a;
		point_light_attenuation.b = b;
		point_light_attenuation.c = c;
	};

	PointLight  pointLight;
	Attenuation point_light_attenuation;	
	Sphere		light_view;

	int view_ambient  = true;
	int view_diffuse  = true;
	int view_specular = true; 
};

#endif
