#version 330 core

in  vec2 frag_tex_coord;
out vec4 out_colour;

/*************************************************************************************/

struct pointLight {
	vec3  worldPos;		
	vec3  lightIntensity;		
	vec3  ambientIntensity;	
	float specularPower;
};

struct material {
	vec3 ambientMaterial;		
	vec3 diffuseMaterial;	
	vec3 specularMaterial;
};
 
struct fragData {
	vec3 worldPos;
	vec3 normal;
	vec4 colour;
};

struct atten_t {
	float a, b, c;
};  
 
/*************************************************************************************/

uniform sampler2D pos_tex;
uniform sampler2D normal_tex;
uniform sampler2D colour_tex;

uniform vec3       gEyeWorldPos; 
uniform pointLight gPointLight;
uniform material   gMaterial;
uniform atten_t    g_atten;

uniform int	g_view_ambient; 
uniform int	g_view_diffuse; 
uniform int	g_view_specular;
	
/*************************************************************************************/

vec3 calcDiffuseLight(material m , vec3 light_vec, 
					  vec3 normal, vec3 light_inten) 
{
	float similiarity    = dot(light_vec, normal);
	float diffuse_factor = max(similiarity, 0.0f);

	return diffuse_factor * m.diffuseMaterial * light_inten; 	
}
  
/*************************************************************************************/

vec3 calcSpecularLight(material m, vec3 light_vec, vec3 normal, 
					   vec3 V, vec3 light_inten)
{
	vec3 H = normalize(light_vec + V);
	float similiarity = max(dot(H, normal), 0.0f);

	float spec_factor = pow(similiarity, gPointLight.specularPower);
	return spec_factor * m.specularMaterial * light_inten;
}

/*************************************************************************************/

vec3 calcPointLight(material m,  pointLight l, fragData frag)  
{ 
	vec3 light_vec  = normalize(l.worldPos - frag.worldPos);
	vec3 normal_vec = normalize(frag.normal); 
	vec3 amb_light  = vec3(0.0f, 0.0f, 0.0f);
	vec3 diff_light = vec3(0.0f, 0.0f, 0.0f);
	vec3 spec_light = vec3(0.0f, 0.0f, 0.0f);

	if (g_view_ambient == 1)
		amb_light = l.lightIntensity * m.ambientMaterial * l.ambientIntensity;

	if (g_view_diffuse == 1)
		diff_light = calcDiffuseLight(m, light_vec, normal_vec, l.lightIntensity);

	if (g_view_specular == 1) {
		vec3 view  = normalize(gEyeWorldPos - frag.worldPos);
		spec_light = calcSpecularLight(m, light_vec, normal_vec, view, l.lightIntensity);
	}
	
	float dist         = distance(l.worldPos, frag.worldPos);
	float atten_factor = g_atten.a * 10.0f;

	vec3 colour = (atten_factor * (amb_light + diff_light + spec_light)) * frag.colour.rgb;			
	colour.r    = min(colour.r, 1.0f);
	colour.g    = min(colour.g, 1.0f);
	colour.b    = min(colour.b, 1.0f);
	
	return colour;
}

/*************************************************************************************/

void main() 
{
	fragData frag;
	vec3     frag_colour;

	frag.worldPos = texture(pos_tex   , frag_tex_coord).xyz;
	frag.normal   = texture(normal_tex, frag_tex_coord).xyz;
	frag.colour   = texture(colour_tex, frag_tex_coord);

	frag_colour = calcPointLight(gMaterial, gPointLight, frag);
	out_colour  = vec4(frag_colour, 1.0f); 
}
