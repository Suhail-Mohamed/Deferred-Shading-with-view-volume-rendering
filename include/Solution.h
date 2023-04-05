#pragma once

#include "nuss_vector.h"
#ifndef SOLUTION_HEADER
#define SOLUTION_HEADER

#include <sys/types.h>
#include <stdlib.h>
#include <time.h>  
#include <string>

#include <GL/glew.h>
#include <GL/glut.h>

#include "Shader.h"
#include "sphere.h"
#include "camera.h"
#include "Light.h"
#include "../include/nuss_math.h"

/*****************************************************************************************/

#define UPDATE_RENDERRED_OBJECTS 1000
#define FRAME_TIME				 30
#define NUM_LIGHTS				 150
#define NUM_SPHERES				 10
#define WINDOW_SIZE				 1024
#define MOVEMENT_SPEED			 3
#define NUM_CORNERS              4

/************************************************************************************************/

struct framebuffer_t {
	unsigned int fbo,
				 pos_tex, 
				 normal_tex, 
				 colour_tex, 
				 rbo;

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	
	void delete_frame_buffer() {
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &pos_tex);
		glDeleteTextures(1, &normal_tex);
		glDeleteTextures(1, &colour_tex);
		glDeleteRenderbuffers(1, &rbo);

		pos_tex = normal_tex = colour_tex = rbo = 0;
	}

	void init_frame_buffer() {
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &pos_tex);
		glBindTexture(GL_TEXTURE_2D, pos_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_SIZE, WINDOW_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pos_tex, 0);
	
		glGenTextures(1, &normal_tex);
		glBindTexture(GL_TEXTURE_2D, normal_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WINDOW_SIZE, WINDOW_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal_tex, 0);
	
		glGenTextures(1, &colour_tex);
		glBindTexture(GL_TEXTURE_2D, colour_tex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_SIZE, WINDOW_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colour_tex, 0);
	
		glDrawBuffers(3, attachments);
		
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINDOW_SIZE, WINDOW_SIZE);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "FRAMEBUFFER NOT COMPLETE\n";
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

/************************************************************************************************/

class Solution
{
public:
	Solution();
	~Solution();
	int initOpenGL();

	static void renderCB();
	static void keyboardCB(unsigned char key, int x, int y);
	static void specialKeyboardCB(int key, int x, int y);
	static void winResizeCB(int width, int height);
	static void timerCB(int operation);
	static void setSolution(Solution * _sol);

	int initSolution();

	float random_number (int lower_bnd, int upper_bnd) {
		return ((upper_bnd - lower_bnd) * ((float)rand() / RAND_MAX)) + lower_bnd;
	};
	
	Vector3f random_vec (int lower_bnd, int upper_bnd) {
		return Vector3f {random_number(lower_bnd, upper_bnd),
		                 random_number(lower_bnd, upper_bnd),		
						 random_number(lower_bnd, upper_bnd)};
	}


private:
	Shader shader_fbuffer, 
		   shader_phongtex,
		   shader_basic,
		   shader_null;
	Sphere sphere;
	Sphere sphere_list[NUM_SPHERES];
	Camera cam;
	
	int  numFrames;
	int  factor;	
	bool render_volumes    = false;
	bool show_corners_only = false;

	static Solution *sol;
	
	void stencil_lighting_pass(size_t idx);
	void render(); void keyboard(unsigned char key, int x, int y);
	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int  timer(int operation);
	
	int  updateObjects(int numFrames);
	int  printOpenGLError(int errorCode);

	Light  light_list[NUM_LIGHTS];
	Sphere view_light[NUM_LIGHTS];
	int	   corners[NUM_CORNERS] = {0, 9, NUM_LIGHTS - 10, NUM_LIGHTS - 1};
	
	framebuffer_t f_buffer;
	char* fbuffer_vtx_shader  = const_cast<char*>("../shaders/frame_buffer.vert");
	char* fbuffer_frg_shader  = const_cast<char*>("../shaders/frame_buffer.frag");
	char* phongtex_vtx_shader = const_cast<char*>("../shaders/phong_texture_shading.vert");
	char* phongtex_frg_shader = const_cast<char*>("../shaders/phong_texture_shading.frag");
	char* basic_vtx_shader    = const_cast<char*>("../shaders/basic_shader.vert");
	char* basic_frg_shader    = const_cast<char*>("../shaders/basic_shader.frag");
	char* null_vtx_shader     = const_cast<char*>("../shaders/null_shader.vert");
	char* null_frg_shader       = const_cast<char*>("../shaders/null_shader.frag");
};

#endif
