#pragma once

#include <string>
#include "GL/glew.h"
//#include "vec2.h"
//#include "vec3.h"
//#include "vec4.h"
#include "glm/glm.hpp"

#define _DEBUG
using std::string;

class GLShader {
public:
	GLShader();
	~GLShader();

	GLuint loadFiles(const std::string& vertex_filename, const std::string& fragment_filename);
	GLuint load(const std::string& vertex_sourse, const std::string& fragment_sourse);
	GLuint load(const GLchar* vertex_source, const GLchar* fragment_source);

	void use();
	void release();

	GLuint getIDProgram();

	bool isLoad();

	GLint getAttribLocation(const GLchar* name) const;
	GLint getAttribLocation(const std::string& name) const;
	
	GLint getUniformLocation(const GLchar* name) const;
	GLint getUniformLocation(const std::string& name) const;

	void setUniform(GLint location, const glm::vec4& value);
	void setUniform(GLint location, const glm::vec3& value);
	void setUniform(GLint location, const glm::vec2& value);

	void setUniform(GLint location, const glm::mat4& value);
	void setUniform(GLint location, const GLint value);
	void setUniform(GLint location, const GLfloat value);
private:
	void printInfoLogShader(GLuint shader);
	void printInfoLogProgram(GLuint shader);

	GLuint loadSourcefile(const std::string& source_file_name, GLuint shader_type);

	GLuint compileSource(const GLchar* source, GLuint shader_type);

	void linkProgram();

	GLuint ShaderProgram;
	GLuint vertex_shader;
	GLuint fragment_shader;
};