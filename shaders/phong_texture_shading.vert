#version 330 core

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection;

in vec4 vtxPos;

void main() 
{
	gl_Position = projection * view * model * vtxPos;
}
