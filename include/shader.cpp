#include "shader.h"

Shader::Shader(const char* vsFilePath, const char* fsFilePath)
{
	//read vertex/fragment shader source code from filepath
	std::string vsSrc, fsSrc;
	try
	{
		std::ifstream vsFile(vsFilePath);
		if (!vsFile)
		{
			std::cout << "Open vertex shader file failed! -> " << vsFilePath << std::endl;
		}

		std::ifstream fsFile(fsFilePath);
		if (!fsFile)
		{
			std::cout << "Open fragment shader file failed! -> " << fsFilePath << std::endl;
		}

		std::stringstream vsStream, fsStream;
		vsStream << vsFile.rdbuf();
		fsStream << fsFile.rdbuf();
		vsFile.close();
		fsFile.close();
		vsSrc = vsStream.str();
		std::cout << vsFilePath << std::endl << vsSrc << std::endl;
		fsSrc = fsStream.str();
		std::cout << fsFilePath << std::endl << fsSrc << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	const char* vsCode = vsSrc.c_str();
	const char* fsCode = fsSrc.c_str();

	//compile shaders
	unsigned int vs, fs;
	
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsCode, NULL);
	glCompileShader(vs);
	checkCompileErrors(vs, "VERTEX");

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsCode, NULL);
	glCompileShader(fs);
	checkCompileErrors(fs, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vs);
	glAttachShader(ID, fs);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader() {}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(getUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(getUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(getUniformLocation(name), value);
}

void Shader::checkCompileErrors(unsigned int shader, const std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

const GLint &Shader::getUniformLocation(const std::string & name) const
{
	return glGetUniformLocation(this->ID, name.c_str());
}


void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(getUniformLocation(name), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{
	glUniform4f(getUniformLocation(name), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &value) const
{
	glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &value) const
{
	glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &value) const
{
	glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

