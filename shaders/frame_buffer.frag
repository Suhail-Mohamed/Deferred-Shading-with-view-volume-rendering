#version 330 core 
 
in vec3 world_pos; 
in vec3 normal;
in vec4 colour;

out vec3 pos_tex;
out vec3 normal_tex;
out vec4 colour_tex;

void main() 
{
	pos_tex    = world_pos;
	normal_tex = normalize(normal);
	colour_tex = colour; 
}


