#pragma once


#include <iostream>
#include "GL/glew.h"
#include <string>

#include "Viewer.h"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"
#include "fboManager.h"
#include "textureManager.h"

struct Parameter {
	float light_x;
	float light_y;
	float light_z;
	bool depth;
};

class MyGlWindow {
public:
	MyGlWindow(float w, float h);
	~MyGlWindow();
	void draw(Parameter param);
	void resize(int width, int height);

	Viewer *m_viewer;

private:

	float m_width;
	float m_height;

	//Shaders
	Shader *wrap_shader;
	Shader *shadow_map;
	Shader *debug_shader;

	//Models
	Mesh *dragon;

	unsigned int fbo;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMap;
	
	void setupBuffer();
	void renderQuad();

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
};