#pragma once

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

#include <iostream>
#include <string>

#include "Shader.h"
#include "Viewer.h"

class MyWindow {
public:
	MyWindow(float w, float h);
	~MyWindow();
	void draw();
	void resize(int width, int height);

	Viewer *_viewer;

private:

	//Shaders

	Viewer _viewer;
	float _width;
	float _height;

	//Objects

	void setupBuffer();
};