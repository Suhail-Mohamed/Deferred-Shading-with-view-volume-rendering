#include "Solution.h"

#include "../include/nuss_math.h"
#include <ctime>
#include <iostream>
#include "time.h"



#define NORMAL_SPEED 0.5
#define MAX_SPEED 2.0


/************************************************************/

#define NO_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT  2
#define DIRECTIONAL_LIGHT 3

/************************************************************/

Solution *Solution::sol = NULL;

/****************************************************************************/

Solution::Solution() : numFrames(0)
{
}
 
/*************************************************************************/

Solution::~Solution()
{
	printf("\n exiting the progam gracefully\n");

	render_surface.delete_surface();
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
	glEnable(GL_DEPTH_TEST);

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
	
	auto random_number = [](int lower_bnd, int upper_bnd) -> float 
	{
		return lower_bnd + static_cast<float>(rand()) /
			   (static_cast<float>(RAND_MAX/(upper_bnd - lower_bnd)));
	};

	if (shader_fbuffer.createShaderProgram(fbuffer_vtx_shader, fbuffer_frg_shader) != 0) {
		std::cout << "Error generating frame buffer shader (see solution)\n";
		return -1;
	}

	if (shader_phongtex.createShaderProgram(phongtex_vtx_shader, phongtex_frg_shader) != 0) {
		std::cout << "Error generating phong texture shader (see solution)\n";
		return -1;
	}
	
	shader_phongtex.useProgram(true);
	shader_phongtex.copy_integer_to_shader("pos_tex"   , 0);
	shader_phongtex.copy_integer_to_shader("normal_tex", 1);
	shader_phongtex.copy_integer_to_shader("colour_tex", 2);

	f_buffer.init_frame_buffer();
	render_surface.init_surface(shader_phongtex, "vtxPos", "tex_coord");
	
	light.pointLight.lightIntensity   = {10.0f, 10.0f, 10.0f};
	light.pointLight.ambientIntensity = {3.0f, 3.0f, 3.0f};
	light.pointLight.worldPos         = {140.0f, 0.0f, 150.0f};
	light.pointLight.specularPower    = 10.0f;
	
	/*
	light.light_view.setInitialPosition(light.pointLight.worldPos);
	light.light_view.createSphere(5, 5, vtx2, ind2);
	light.light_view.createVAO(shader_fbuffer, vtx2, ind2);
	light.light_view.setInitialPosition(light.pointLight.worldPos);
	light.light_view.setScale(10.0f, 10.0f, 10.0f);
	light.light_view.sphere_colour = {0.5f, 0.5f, 0.5f, 1.0f};
	*/

	sphere.materials.ambientMaterial  = {0.2f, 0.2f, 0.2f};
	sphere.materials.diffuseMaterial  = {0.8f, 0.8f, 0.8f};
	sphere.materials.specularMaterial = {1.0f, 1.0f, 1.0f};
	sphere.sphere_colour              = {1.0f, 0.0f, 0.0f, 1.0f};	
	sphere.createSphere(5, 5, vtx1, ind1);
	sphere.createVAO(shader_fbuffer, vtx1, ind1);
	sphere.setInitialPosition(150, 0, 100);
	sphere.setInitialRotations(0, 0, 0);
	sphere.setScale(40.0f, 40.0f, 40.0f);

	cam.setCamera(Vector3f(200, 100, 400), Vector3f(145, 0, 0), Vector3f(0, 1, 0));
	return 0;
}

/**********************************************************************/

void Solution::setSolution(Solution * _sol)
{
	Solution::sol = _sol;
}

/************************************************************/

void Solution::load_point_light_list(Shader shader)
{
	auto load_light = [&shader](std::string light_arr, std::string atten_arr, 
				                pointLight& cur_light, Attenuation& cur_atten) -> void 
	{
		std::string l_inten = light_arr + ".lightIntensity";
		std::string w_pos   = light_arr + ".worldPos";
		std::string a       = atten_arr + ".a";
		std::string b       = atten_arr + ".b";
		std::string c       = atten_arr + ".c";
		
		shader.copyFloatVectorToShader((float*)&cur_light.lightIntensity, 1, 3, l_inten.c_str());
		shader.copyFloatVectorToShader((float*)&cur_light.worldPos      , 1, 3, w_pos.c_str());
		shader.copyFloatVectorToShader((float*)&cur_atten.a		        , 1, 1, a.c_str());
		shader.copyFloatVectorToShader((float*)&cur_atten.b		        , 1, 1, b.c_str());
		shader.copyFloatVectorToShader((float*)&cur_atten.c		        , 1, 1, c.c_str());
	};

	for (size_t i = 0; i < NUM_LIGHTS; ++i) {
		std::string light_arr = "g_point_light_list[" + std::to_string(i) + "]";
		std::string atten_arr = "g_atten_list[" + std::to_string(i) + "]";
		load_light(light_arr, atten_arr, 
			   point_light_list[i].pointLight, 
			   point_light_list[i].point_light_attenuation);
	}
}

void Solution::render()
{
	Vector3f cam_pos = cam.getPosition(); 
	Matrix4f viewMat = cam.getViewMatrix(nullptr);	
	Matrix4f projMat = cam.getProjectionMatrix(nullptr);
	
	/* geometry pass of deffered shading */
	glBindFramebuffer(GL_FRAMEBUFFER, f_buffer.fbo);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_fbuffer.useProgram(true);
	shader_fbuffer.copyMatrixToShader(viewMat, "view");
	shader_fbuffer.copyMatrixToShader(projMat, "projection");
	
	sphere.render(shader_fbuffer);

	/* lighting pass of deffered shading */
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	shader_phongtex.useProgram(true);

	sphere.loadMaterials(shader_phongtex);
	light.loadPointLight(shader_phongtex);
	shader_phongtex.copyFloatVectorToShader((float*)&cam_pos, 1, 3, "gEyeWorldPos");
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f_buffer.pos_tex);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, f_buffer.normal_tex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, f_buffer.colour_tex);

	render_surface.draw_surface();
	
	/* loading the depth information attained from the geometry pass to be used
	   for normal forward rendering if desired */
	glBindFramebuffer(GL_READ_FRAMEBUFFER, f_buffer.fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, WINDOW_SIZE, WINDOW_SIZE, 0, 0, 
					  WINDOW_SIZE, WINDOW_SIZE, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
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
		case 'p':
			plotFill == 1 ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE): 
					        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			plotFill *= -1;
			break;
		case 'r':
			cam.roll(1.0f);
			break;
		case 'j':
			cam.moveForward(1.0f);
			break;
		case 'k':
			cam.moveBackward(1.0f);
			break;
		case 'A':
			std::cout << "Ambient Light off\n";
			light.view_ambient = false;
			break;
		case 'a':
			std::cout << "Ambient Light on\n";
			light.view_ambient = true;
			break;
		case 'D':
			std::cout << "Diffuse Light on\n";
			light.view_diffuse = false;
			break;
		case 'd':
			std::cout << "Diffuse Light on\n";
			light.view_diffuse = true;
			break;
		case 'S':
			std::cout << "Specular Light off\n";
			light.view_specular = false;
			break;
		case 's':
			std::cout << "Specular Light on\n";
			light.view_specular = true;
			break;
		case '+':
			light.pointLight.specularPower = std::min(50.0f, light.pointLight.specularPower + 2);
			break;
		case '-':
			light.pointLight.specularPower = std::max(1.0f, light.pointLight.specularPower - 2);
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
			cam.yaw(1.0f);
			break;
		case GLUT_KEY_UP:
			cam.pitch(1.0f);
			break;
		case GLUT_KEY_RIGHT:
			cam.yaw(-1.0f);
			break;
		case GLUT_KEY_DOWN:
			cam.pitch(-1.0f);
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
	sphere.incrementRotations(0, 0, 0.5);
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
