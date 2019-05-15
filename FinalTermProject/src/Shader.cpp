#include "Shader.h"

Shader::Shader(const std::string &vertexPath, const std::string &fragPath)
{
	std::string vertexCode = loadShaderFromFile(vertexPath);
	std::string fragCode = loadShaderFromFile(fragPath);

	GLuint vertexShaderID = compileShader(vertexCode, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = compileShader(fragCode, GL_FRAGMENT_SHADER);

	programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	GLint programLinkSuccess = GL_FALSE;
	glGetProgramiv(programID, GL_LINK_STATUS, &programLinkSuccess);
	if (programLinkSuccess == GL_TRUE)
	{
		std::cout << "Shader program link successful." << std::endl;
	}
	else
	{
		std::cout << "Shader program link failed: " << getInfoLog(ObjectType::PROGRAM, programID) << std::endl;
	}
}

Shader::~Shader()
{
	glDeleteProgram(programID);
}

std::string Shader::loadShaderFromFile(const std::string &filename)
{
	std::ifstream file(filename);

	if (!file.good())
	{
		std::cout << "Failed to open file : " << filename << std::endl;
	}

	std::stringstream  stream;
	stream << file.rdbuf();
	file.close();

	return stream.str();
}

GLuint Shader::compileShader(std::string &shaderSource, GLenum shaderType)
{
	GLuint shader;
	std::string shaderTypeString;
	const char *shaderCode;

	switch (shaderType)
	{
	case GL_VERTEX_SHADER:
		shaderTypeString = "GL_VERTEX_SHADER";
		break;
	case GL_FRAGMENT_SHADER:
		shaderTypeString = "GL_FRAGMENT_SHADER";
		break;
	default:
		throw std::runtime_error("Bad shader type enum in compileShader");
		break;
	}
	
	shaderCode = shaderSource.c_str();
	if ((shader = glCreateShader(shaderType)) == 0)
	{
		throw std::runtime_error("Could not create shader of type " + shaderTypeString +
			": " + getInfoLog(ObjectType::SHADER, shader));
	}
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_FALSE)
	{
		std::cout << shaderTypeString << " compilation failed: " 
			<< getInfoLog(ObjectType::SHADER, shader) << std::endl;
	}

	return shader;
}

std::string Shader::getInfoLog(ObjectType type, int id)
{
	GLint infoLogLength;
	if (type == ObjectType::SHADER)
	{
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else
	{
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	GLchar *infoLog = new GLchar[infoLogLength + 1];
	if (type == ObjectType::SHADER)
	{
		glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
	}
	else
	{
		glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);
	}

	std::string infoLogString(infoLog);
	delete[] infoLog;

	return infoLogString;
}

void Shader::use()
{
	glUseProgram(programID);
}

void Shader::disable()
{
	glUseProgram(0);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}