#pragma once


#include <iostream>
#include "GL/glew.h"
#include <string>

#include "Viewer.h"
#include "glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "Shader.h"

struct Parameter {
	float light_x;
	float light_y;
	float light_z;
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

	//Models
	Mesh *dragon;

	void setupBuffer();
};