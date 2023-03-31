#pragma once

#ifndef LIGHT_HEADER
#define LIGHT_HEADER

/***************************************************************************/

#include "../include/nuss_vector.h"
#include "Shader.h"
#include "sphere.h"

/***************************************************************************/

#define MAX_INTENSITY 10

typedef struct pointLight {
	Vector3f lightIntensity;
	Vector3f ambientIntensity;	
	Vector3f worldPos;			
	float    specularPower;	
} PointLight;

/* attenuation calculated as 1 / (c + b * d + a * d^2) */
typedef struct attenuation {
	float a; /* quadratic component */
	float b; /* linear component    */
	float c; /* constant component  */
} Attenuation;


struct Light {
	Light() {
		atten.a = 0.05f;
		atten.b = 0.01f;
		atten.c = 0.01f;
	}

	int loadPointLight(Shader shader) {
		shader.copyFloatVectorToShader((float*)&pointLight.ambientIntensity, 1, 3, "gPointLight.ambientIntensity");
		shader.copyFloatVectorToShader((float*)&pointLight.lightIntensity  , 1, 3, "gPointLight.lightIntensity");
		shader.copyFloatVectorToShader((float*)&pointLight.worldPos        , 1, 3, "gPointLight.worldPos");
		shader.copyFloatVectorToShader((float*)&pointLight.specularPower   , 1, 1, "gPointLight.specularPower");
		
		shader.copyFloatVectorToShader((float*)&atten.a, 1, 1, "g_atten.a");
		shader.copyFloatVectorToShader((float*)&atten.b, 1, 1, "g_atten.b");
		shader.copyFloatVectorToShader((float*)&atten.c, 1, 1, "g_atten.c");

		/* load which light components to use */
		shader.copyIntVectorToShader((int*)&view_ambient , 1, 1, "g_view_ambient");
		shader.copyIntVectorToShader((int*)&view_diffuse , 1, 1, "g_view_diffuse");
		shader.copyIntVectorToShader((int*)&view_specular, 1, 1, "g_view_specular");

		return 0;
	}
	
	void set_attenuation(int a,int b, int c) {
		atten.a = a;
		atten.b = b;
		atten.c = c;
	}

	float calculate_bound_sphere() {
		Vector3f light_inten = pointLight.lightIntensity;	
		float max_channel    = fmax(fmax(light_inten.r, light_inten.g), light_inten.b);

		return (-atten.b + sqrtf(atten.b * atten.b - 4 * atten.a * 
			   (atten.a - 256 - max_channel * pointLight.specularPower))) / (2 * atten.a); 
	}
	
	Vector4f light_colour() {
		return Vector4f((1.0f / MAX_INTENSITY) * pointLight.lightIntensity, 1.0f);
	}

	PointLight  pointLight;
	Attenuation atten;	
	Sphere		light_view;

	int view_ambient  = true;
	int view_diffuse  = true;
	int view_specular = true; 
};

#endif
