#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:

	Shader(const std::string &vertexPath, const std::string &fragmentPath);
	~Shader();
	void use();
	void disable();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:

	enum class ObjectType
	{
		SHADER, PROGRAM
	};

	GLuint programID;

	std::string loadShaderFromFile(const std::string &filename);
	GLuint compileShader(std::string &shaderSource, GLenum shaderType);
	std::string getInfoLog(ObjectType type, int id);
};