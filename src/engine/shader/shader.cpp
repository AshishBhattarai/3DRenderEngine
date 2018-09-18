#include "shader.hpp"

#include <fstream>
#include <sstream>

#include "utils/slogger.hpp"

// read a shade file
std::string Shader::readFile(std::string_view path) {
	std::stringstream ss;
	std::ifstream file(std::string(path).c_str());
	// read from file
	if(file.is_open()) {
		ss << file.rdbuf();
		file.close();
	}
	// checkf or errors
	if(file.fail()) {
		SLOG("Failed to read form shader file.", path);
		return std::string("");
	}
	return ss.str();
}

// cerate & compile the shader file
GLuint Shader::createShader(std::string_view path, GLenum type) {
	GLuint shader = 0;
	int success = 0;
	char infoLog[1024];
	std::string type_str;

	// read source
	std::string strSource = readFile(path);
	const char* source = strSource.c_str();
	// create shader
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	// compilehsader
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		type_str = (type == GL_VERTEX_SHADER)? "VERTEX_SHADER" :
							 (type == GL_FRAGMENT_SHADER)? "FRAGMENT_SHADER" :
							 "GEOMETRY_SHADER";
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		SLOG("Failed to compile ", type_str, ". ", infoLog);
		glDeleteShader(shader);
		shader = 0;
	}
	return shader;
}

// gemotryShader is optional
Shader::Shader(std::string_view vertexPath, std::string_view fragmentPath,
		std::string_view geometryPath) :
		program(0),
		hasGeomShader(false)
{
	// check if it has geometry shader
	hasGeomShader = !geometryPath.empty();
	// create shaders
	GLuint vertexShader = createShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = createShader(fragmentPath, GL_FRAGMENT_SHADER);
	GLuint geometryShader = 0;
	if(hasGeomShader)
		geometryShader = createShader(geometryPath, GL_GEOMETRY_SHADER);
	// create program
	program = glCreateProgram();
	// attach shaders
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	if(hasGeomShader)
		glAttachShader(program, geometryShader);
	// link program
	glLinkProgram(program);
	// cleanup - you can delete shaders after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if(hasGeomShader)
		glDeleteShader(geometryShader);

	// check for link errors
	int success = 0;
	char infoLog[1024];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		SLOG("Failed to link shader program.", infoLog);
		glDeleteProgram(program);
		program = 0;
	}
}

Shader::~Shader() {
	// clean up
	glUseProgram(0);
	glDeleteProgram(program);
}

void Shader::start() {
	glUseProgram(program);
}
void Shader::stop() {
	glUseProgram(0);
}

GLint Shader::getUniformLoc(const char* name) const {
	GLint loc = glGetUniformLocation(program, name);
	if (loc == -1)
		SLOG("failed to get location for ", name);
	return loc;
}

void Shader::setUniformBlockBinding(const char* name, u_int point) {
	// get block index
	GLuint index = glGetUniformBlockIndex(program, name);
	// check for errors
	if(index == GL_INVALID_INDEX) {
		CSLOG("Invalid block index for, ", name);
		return;
	}
	// set block bindingpoint
	glUniformBlockBinding(program, index, point);
}