#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string_view>

#include "utils/slogger.hpp"

class Shader {
private:
	GLuint program;
	bool hasGeomShader;
	// read a shade file
	std::string readFile(std::string_view path);
	// cerate & compile the shader file
	GLuint createShader(std::string_view path, GLenum type);

public:
	Shader(std::string_view vertexPath, std::string_view fragmentPath,
		std::string_view geometryPath = ""); // gemotryShader is optional
	~Shader();

	void start();
	void stop();

	GLint getUniformLoc(const char* name) const;

	// set binding point for the number block name
	void setUniformBlockBinding(const char* name, u_int point);

	// load uniforms
	void uniform1i(int loc, int value) {
		glUniform1i(loc, value);
	}

	void uniform1f(int loc, float value) {
		glUniform1f(loc, value);
	}

	void uniform3f(int loc, float x, float y, float z) {
		glUniform3f(loc, x, y, z);
	}

	void uniform3fv(int loc, const glm::vec3& value) {
		glUniform3fv(loc, 1, glm::value_ptr(value));
	}

	// void uniform3fv(int loc, glm::vec3&& value) {
	// 	glUniform3fv(loc, 1, glm::value_ptr(value));
	// }

	void uniform4fv(int loc, const glm::vec4& value) {
		glUniform4fv(loc, 1, glm::value_ptr(value));
	}

	void uniformMatrix3fv(int loc, const glm::mat3& value) {
		glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void uniformMatrix4fv(int loc, const glm::mat4& value) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
	}

	void loadFloat(const char* name, float val) {
		GLint loc = getUniformLoc(name);
		glUniform1f(loc, val);
	}

	void loadInt(const char* name, int val) {
		GLint loc = getUniformLoc(name);
		glUniform1i(loc, val);
	}

	void loadVec3(const char* name, glm::vec3 value) {
		GLint loc = getUniformLoc(name);
		uniform3fv(loc, value);
	}

	void loadMatrix4(const char* name, glm::mat4 value) {
		GLint loc = getUniformLoc(name);
		uniformMatrix4fv(loc, value);
	}

	void loadMatrix3(const char* name, glm::mat3 value) {
		GLint loc = getUniformLoc(name);
		uniformMatrix3fv(loc, value);
	}
};

#endif