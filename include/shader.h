#pragma once

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>

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

	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;

	void setMat2(const std::string &name, const glm::mat2 &value) const;
	void setMat3(const std::string &name, const glm::mat3 &value) const;
	void setMat4(const std::string &name, const glm::mat4 &value) const;

private:
	void checkCompileErrors(unsigned int shader, const std::string type);

	const GLint &getUniformLocation(const std::string & name) const;
};

#endif // !__SHADER_H__
