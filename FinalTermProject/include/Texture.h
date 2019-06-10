#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Texture {
public:

	enum TextureType {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};

	Texture(const std::string &file, std::string dir, TextureType type);
	~Texture();

	GLuint getId() const;
	TextureType getType() const;
	std::string getPath() const;

private:

	std::string _path;
	GLuint _id;
	TextureType _type;

};