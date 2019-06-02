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
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else
	{
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	GLchar *infoLog = new GLchar[infoLogLength + 1];
	if (type == ObjectType::SHADER)
	{
		glGetShaderInfoLog(programID, infoLogLength, NULL, infoLog);
	}
	else
	{
		glGetProgramInfoLog(programID, infoLogLength, NULL, infoLog);
	}

	std::string infoLogString(infoLog);
	delete[] infoLog;

	return infoLogString;
}

GLuint Shader::getId() const
{
	return programID;
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
	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else 
		glUniform1i(uniform, (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform1i(uniform, value);
}

void Shader::setFloat(const std::string &name, float value) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform1f(uniform, value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform2fv(uniform, 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform2f(uniform, x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform3fv(uniform, 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform3f(uniform, x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform4fv(uniform, 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniform4f(uniform, x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniformMatrix2fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniformMatrix3fv(uniform, 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{

	GLuint uniform = glGetUniformLocation(programID, name.c_str());
	if (uniform == -1)
		std::cout << "Could not find uniform " + name + ", location = -1" << std::endl;
	else
		glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}
