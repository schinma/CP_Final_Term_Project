#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

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
	GLuint getId() const;
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
	

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