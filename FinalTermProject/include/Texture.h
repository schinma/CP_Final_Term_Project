#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


class Texture {
public:

	enum TextureType {
		DIFFUSE,
		SPECULAR,
		NORMAL,
		HEIGHT
	};

	Texture(const std::string &file, std::string directory, TextureType type) : _type(type)
	{
		_path = file;
		std::string  fullPath = directory + "/" + _path;

		std::cout << "Loading texture lololo:" + file + " at path :" + fullPath << std::endl;

		int width, height, channel;
		unsigned char *texture_img = stbi_load(fullPath.c_str(), &width, &height, &channel, 0);

		if (texture_img) {
			glActiveTexture(GL_TEXTURE0);
			glGenTextures(2, &_id);
			glBindTexture(GL_TEXTURE_2D, _id);

			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, texture_img);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

			stbi_image_free(texture_img);
		}
		else {
			std::cout << "Texture " + file + " failed to be loaded at path: " + fullPath << std::endl;
		}
	}

	~Texture() {}

	GLuint getId() const
	{
		return _id;
	}

	TextureType getType() const
	{
		return _type;
	}

	std::string getPath() const
	{
		return _path;
	}

private:

	std::string _path;
	GLuint _id;
	TextureType _type;

};