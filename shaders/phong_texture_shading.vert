#version 330 core

in vec3 vtxPos;
in vec2 tex_coord;

out vec2 frag_tex_coord;

void main() 
{
	frag_tex_coord = tex_coord;
	gl_Position    = vec4(vtxPos, 1.0f);
}
