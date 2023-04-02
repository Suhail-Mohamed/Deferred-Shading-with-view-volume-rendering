#include "Solution.h"

#include "../include/nuss_math.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "nuss_vector.h"
#include "time.h"


/************************************************************/

#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0
#define NO_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT  2
#define DIRECTIONAL_LIGHT 3

/************************************************************/

Solution *Solution::sol = NULL;

Solution::Solution() : numFrames(0)
{
	srand48(time(nullptr));
}
 
/*************************************************************************/

Solution::~Solution()
{
	printf("\n exiting the progam gracefully\n");
	f_buffer.delete_frame_buffer();
}

/******************************************************************************/

int Solution::initOpenGL()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
	glutCreateWindow("Deffered Shading");
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutDisplayFunc(Solution::renderCB);
	glutReshapeFunc(Solution::winResizeCB);
	glutKeyboardFunc(Solution::keyboardCB);
	glutSpecialFunc(Solution::specialKeyboardCB);
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error - %s \n", glewGetErrorString(res));
		return (-1);
	}

	return 0;
}

/******************************************************************************/

void Solution::renderCB()
{
	sol->render();
}


/************************************************************/

void Solution::keyboardCB(unsigned char key, int x, int y)
{
	sol->keyboard(key, x, y);
}


/************************************************************/

void Solution::specialKeyboardCB(int key, int x, int y)
{
	sol->specialKeyboard(key, x, y);
}


/************************************************************/

void Solution::winResizeCB(int width, int height)
{
	sol->winResize(width, height);
}

/************************************************************/

void Solution::timerCB(int operation)
{
	glutTimerFunc(FRAME_TIME, Solution::timerCB, UPDATE_RENDERRED_OBJECTS);	
	sol->timer(operation);
}


/************************************************************/

int Solution::timer(int operation)
{
	switch (operation) {
		case UPDATE_RENDERRED_OBJECTS:
			updateObjects(numFrames);
			break;
		default:
			break;
	}

	return 0;
}

/******************************************************************************/

int Solution::initSolution()
{
	srand(time(NULL));
	
	Vertices vtx1, vtx2;
	Indices  ind1, ind2;

	if (shader_fbuffer.createShaderProgram(fbuffer_vtx_shader, fbuffer_frg_shader) != 0) {
		std::cout << "Error generating frame buffer shader (see solution)\n";
		return -1;
	}

	if (shader_phongtex.createShaderProgram(phongtex_vtx_shader, phongtex_frg_shader) != 0) {
		std::cout << "Error generating phong texture shader (see solution)\n";
		return -1;
	}
	
	if (shader_basic.createShaderProgram(basic_vtx_shader, basic_frg_shader) != 0) {
		std::cout << "Error generating basic  shader (see solution)\n";
		return -1;
	}
	
	shader_phongtex.useProgram(true);
	shader_phongtex.copy_integer_to_shader("pos_tex"   , 0);
	shader_phongtex.copy_integer_to_shader("normal_tex", 1);
	shader_phongtex.copy_integer_to_shader("colour_tex", 2);

	f_buffer.init_frame_buffer();
	Vector3f light_position = {140.0f, 40.0f, 150.0f};
	for (size_t i = 0; i < NUM_LIGHTS; ++i) {
		bool row_mod = (i % 10 == 0);
		if (row_mod) light_position.x = 140.0f;
		light_position.x = light_position.x + 40.0f;
		light_position.z = light_position.z - row_mod * 40.0f;
	
		light_list[i].pointLight.lightIntensity   = {3.0f, 3.0f, 3.0f};
		light_list[i].pointLight.ambientIntensity = {1.0f, 1.0f, 1.0f};
		light_list[i].pointLight.worldPos         = light_position;
		light_list[i].pointLight.specularPower    = 10.0f;
	
		float light_size = light_list[i].calculate_bound_sphere();
		light_list[i].light_view.createSphere(20, 20, vtx2, ind2);
		light_list[i].light_view.createVAO(shader_phongtex, vtx2, ind2);
		light_list[i].light_view.setInitialPosition(light_list[i].pointLight.worldPos);
		light_list[i].light_view.setScale(light_size, light_size, light_size);
		light_list[i].loadPointLight(shader_phongtex);
		
		view_light[i].sphere_colour = light_list[i].light_colour();	
		view_light[i].createSphere(20, 20, vtx2, ind2);
		view_light[i].createVAO(shader_basic, vtx2, ind2);
		view_light[i].setInitialPosition(light_list[i].pointLight.worldPos);
		view_light[i].setScale(2.0f, 2.0f, 2.0f);
	}

	Vector3f sphere_position = {140.0f, 90.0f, 150.0f};
	for (size_t i = 0; i < NUM_SPHERES; ++i) {
		bool row_mod = (i % 10 == 0);
		if (row_mod) sphere_position.x = 140.0f;
		sphere_position.x = sphere_position.x + 90.0f;
		sphere_position.z = sphere_position.z - row_mod * 90.0f;
	
		sphere_list[i].materials.ambientMaterial  = random_vec(0.0f, 1.0f); 
		sphere_list[i].materials.diffuseMaterial  = random_vec(0.0f, 1.0f);
		sphere_list[i].materials.specularMaterial = random_vec(0.0f, 1.0f);
		sphere_list[i].sphere_colour              = {random_vec(0.0f,1.0f), 1.0f};	
			
		sphere_list[i].loadMaterials(shader_phongtex);
		sphere_list[i].createSphere(10, 10, vtx1, ind1);
		sphere_list[i].createVAO(shader_fbuffer, vtx1, ind1);
		sphere_list[i].setInitialPosition(sphere_position);
		sphere_list[i].setInitialRotations(0, 0, 0);
		sphere_list[i].setScale(40.0f, 40.0f, 40.0f);
	}

	cam.setCamera(Vector3f(200, 100, 400), Vector3f(145, 0, 0), Vector3f(0, 1, 0));
	return 0;
}

/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}

/************************************************************/

void Solution::render()
{
	Vector3f cam_pos = cam.getPosition(); 
	Matrix4f viewMat = cam.getViewMatrix(nullptr);	
	Matrix4f projMat = cam.getProjectionMatrix(nullptr);
	
	/* geometry pass of deffered shading */
	glDepthMask(GL_TRUE);
	glBindFramebuffer(GL_FRAMEBUFFER, f_buffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); 
	glDisable(GL_BLEND);
	
	shader_fbuffer.useProgram(true);
	shader_fbuffer.copyMatrixToShader(viewMat, "view");
	shader_fbuffer.copyMatrixToShader(projMat, "projection");

	for (size_t i = 0; i < NUM_SPHERES; ++i)
		sphere_list[i].render(shader_fbuffer);
	
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	/* lighting pass of deffered shading */
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shader_phongtex.useProgram(true);
	shader_phongtex.copyMatrixToShader(viewMat, "view");
	shader_phongtex.copyMatrixToShader(projMat, "projection");
	shader_phongtex.copyFloatVectorToShader((float*)&cam_pos, 1, 3, "gEyeWorldPos");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f_buffer.pos_tex);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, f_buffer.normal_tex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, f_buffer.colour_tex);

	for (size_t i = 0; i < NUM_LIGHTS; ++i) {
		light_list[i].loadPointLight(shader_phongtex);
		light_list[i].light_view.render(shader_phongtex);
	}
		
	glBindFramebuffer(GL_READ_FRAMEBUFFER, f_buffer.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0, 
					  WINDOW_SIZE, WINDOW_SIZE, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);	
	if (render_volumes) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader_basic.useProgram(true);
	shader_basic.copyMatrixToShader(viewMat, "view");
	shader_basic.copyMatrixToShader(projMat, "projection");
	for (size_t i = 0; i < NUM_LIGHTS; ++i)
		view_light[i].render(shader_basic);
	if (render_volumes) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSwapBuffers();
}

/************************************************************/

void Solution::keyboard(unsigned char key, int x, int y)
{
	static int plotFill = 1;
	switch (key) {
		case 'q':
			this->~Solution();
			exit(1);
			break;
		case 'f':
			plotFill == 1 ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE): 
					        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			plotFill *= -1;
			break;
		case 'r':
			cam.roll(MOVEMENT_SPEED);
			break;
		case 'w':
			cam.moveForward(MOVEMENT_SPEED);
			break;
		case 's':
			cam.moveBackward(MOVEMENT_SPEED);
			break;
		case 'a':
			cam.changePoitionDelta(0.0f, MOVEMENT_SPEED, 0.0f);
			break;
		case 'd':
			cam.changePoitionDelta(0.0f, -1.0f * MOVEMENT_SPEED, 0.0f);
			break;
		case 'P':
			std::cout << "Ambient Light on\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_ambient = true;
			break;
		case 'p':
			std::cout << "Ambient Light off\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_ambient = false;
			break;
		case 'O':
			std::cout << "Diffuse Light on\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_diffuse = true;	
			break;
		case 'o':
			std::cout << "Diffuse Light off\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_diffuse = false;	
			break;
		case 'I':
			std::cout << "Specular Light on\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_specular = true;	
			break;
		case 'i':
			std::cout << "Specular Light off\n";
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				light_list[i].view_specular = false;	
			break;
		case 'V':
			std::cout << "View light volumes\n";
			render_volumes = true;
			for (size_t i = 0; i < NUM_LIGHTS; ++i) {
				float light_size = light_list[i].calculate_bound_sphere();
				view_light[i].setScale(light_size, light_size, light_size);
			}
			break;
		case 'v':
			std::cout << "NOT Viewing light volumes\n";
			render_volumes = false;
			for (size_t i = 0; i < NUM_LIGHTS; ++i)
				view_light[i].setScale(2.0f, 2.0f, 2.0f);
			break;
		default:
			std::cout << "KEY IS NOT A COMMAND\n";
			break;
	}
}

/************************************************************/

void Solution::specialKeyboard(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:
			cam.yaw(1.0f * MOVEMENT_SPEED);
			break;
		case GLUT_KEY_UP:
			cam.pitch(1.0f * MOVEMENT_SPEED);
			break;
		case GLUT_KEY_RIGHT:
			cam.yaw(-1.0f * MOVEMENT_SPEED);
			break;
		case GLUT_KEY_DOWN:
			cam.pitch(-1.0f * MOVEMENT_SPEED);
			break;
	}
}

/************************************************************/

void Solution::winResize(int width, int height)
{
	glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);
}

/***********************************************************/

int Solution::updateObjects(int numFrames)
{
	for (size_t i = 0; i < NUM_SPHERES; ++i)
		sphere_list[i].incrementRotations(0, 0, 0.5);
	
	glutPostRedisplay();
	return 0;
}

/*************************************************************************************************************/

int Solution::printOpenGLError(int errorCode)
{
	switch (errorCode) {
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.\n");
			break;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION is generated if program is not a program object. or \n");
			printf("GL_INVALID_OPERATION is generated if program has not been successfully linked. or \n");
			printf("GL_INVALID_OPERATION is generated if location does not correspond to a valid uniform variable location for the specified program object.");
			break;
		default:
			printf("openGL unknown error \n");
	}
	return 0;
}
