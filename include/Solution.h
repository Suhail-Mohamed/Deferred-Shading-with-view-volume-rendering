#pragma once

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
#define FRAME_TIME 30
#define NUM_LIGHTS 30
#define WINDOW_SIZE 1000

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
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WINDOW_SIZE, WINDOW_SIZE);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "FRAMEBUFFER NOT COMPLETE\n";
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

/************************************************************************************************/

struct surface_t {
	unsigned int vao, vbo;
	
	const float surface_vertices[12] = {
		   /* positions */  
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	 	 1.0f, -1.0f, 0.0f,
	};
	
	void delete_surface() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		vao = vbo = 0;
	}

	void init_surface(Shader shader, std::string pos_name) {
		int pos_loc = glGetAttribLocation(shader.getProgId(), pos_name.c_str());
		
		if (pos_loc == -1)
			std::cout << "ERROR FETCHING POSITION: '" << pos_name << "' ATTRIBUTE FROM SHADER\n";

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(surface_vertices), &surface_vertices, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(pos_loc);
		glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void draw_surface() {
		glBindVertexArray(vao);	
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
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

private:
	Shader shader_fbuffer, 
		   shader_phongtex,
		   shader_basic;
	Sphere sphere;
	Camera cam;
	
	int numFrames;
	int factor;	

	static Solution *sol;
	
	void render();
	void keyboard(unsigned char key, int x, int y);
	void specialKeyboard(int key, int x, int y);
	void winResize(int width, int height);
	int  timer(int operation);
	
	int  updateObjects(int numFrames);
	void load_point_light_list(Shader shader);
	int  printOpenGLError(int errorCode);

	Light	      light;
	Light	      point_light_list[NUM_LIGHTS];
	framebuffer_t f_buffer;
	surface_t     render_surface;

	char* fbuffer_vtx_shader  = const_cast<char*>("../shaders/frame_buffer.vert");
	char* fbuffer_frg_shader  = const_cast<char*>("../shaders/frame_buffer.frag");
	char* phongtex_vtx_shader = const_cast<char*>("../shaders/phong_texture_shading.vert");
	char* phongtex_frg_shader = const_cast<char*>("../shaders/phong_texture_shading.frag");
	char* basic_vtx_shader    = const_cast<char*>("../shaders/basic_shader.vert");
	char* basic_frg_shader    = const_cast<char*>("../shaders/basic_shader.frag");
};

#endif
