#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	Shader() = default;
	Shader(const char* vsFilePath, const char* fsFilePath);
	~Shader();

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

private:
	void checkCompileErrors(unsigned int shader, const std::string type);
};

#endif // !__SHADER_H__
