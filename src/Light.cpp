/***************************************************************************/
#include <stdio.h>
#include "stdlib.h"
#include "Light.h"

Light::Light()
{
	point_light_attenuation.a = 0.005f;
	point_light_attenuation.b = 0.001f;
	point_light_attenuation.c = 0.001f;
	
	light_view.materials.ambientMaterial  = {0.0f, 0.0f, 0.0f};
	light_view.materials.diffuseMaterial  = {0.0f, 0.0f, 0.0f};
	light_view.materials.specularMaterial = {0.0f, 0.0f, 0.0f}; 
}

/***************************************************************************/

// Purpose: destructor

Light::~Light()
{
}

/***************************************************************************/
/* 
* Purpose: loads the light infrmation to the shader
* 
*/
int Light::loadPointLight(Shader shader)
{
	shader.copyFloatVectorToShader((float*)&pointLight.ambientIntensity, 1, 3, "gPointLight.ambientIntensity");
	shader.copyFloatVectorToShader((float*)&pointLight.lightIntensity  , 1, 3, "gPointLight.lightIntensity");
	shader.copyFloatVectorToShader((float*)&pointLight.worldPos        , 1, 3, "gPointLight.worldPos");
	shader.copyFloatVectorToShader((float*)&pointLight.specularPower   , 1, 1, "gPointLight.specularPower");
	shader.copyFloatVectorToShader((float*)&point_light_attenuation.a  , 1, 1, "g_atten.a");
	shader.copyFloatVectorToShader((float*)&point_light_attenuation.b  , 1, 1, "g_atten.b");
	shader.copyFloatVectorToShader((float*)&point_light_attenuation.c  , 1, 1, "g_atten.c");

	/* load which light components to use */
	shader.copyIntVectorToShader((int*)&view_ambient , 1, 1, "g_view_ambient");
	shader.copyIntVectorToShader((int*)&view_diffuse , 1, 1, "g_view_diffuse");
	shader.copyIntVectorToShader((int*)&view_specular, 1, 1, "g_view_specular");

	return 0;
}




