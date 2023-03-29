#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

in vec4 vtxPos;
in vec3 vtxNormal;
in vec4 vtxCol;

out vec3 world_pos; 
out vec3 normal;
out vec4 colour;

void main() 
{
	gl_Position = (projection * view * model) * vtxPos;

	vec3 normal_transform = mat3(transpose(inverse(model))) * vtxNormal;
	
	colour    = vtxCol; 
	normal    = normal_transform;
	world_pos = (model * vtxPos).xyz;
}
