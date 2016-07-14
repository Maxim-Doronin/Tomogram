#pragma once

#include <string>
#ifdef _DEBUG
	#include "C:\Users\asus\Tomogram\glew\include\GL\glew.h"
#else
	#include "GL/glew.h"
#endif
#include "glm/glm.hpp"

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
	void setUniform(GLint location, const GLuint value);
	void setUniform(GLint location, const GLfloat value);

	void setUniform(const std::string& name, const glm::vec4& value);
	void setUniform(const std::string& name, const glm::vec3& value);
	void setUniform(const std::string& name, const glm::vec2& value);

	void setUniform(const std::string& name, const glm::mat4& value);
	void setUniform(const std::string& name, const GLint value);
	void setUniform(const std::string& name, const GLuint value);
	void setUniform(const std::string& name, const GLfloat value);

private:
	void printInfoLogShader(GLuint shader, GLuint shader_type);
	void printInfoLogProgram(GLuint shader);
	void versionControl();

	const GLchar* loadSourcefile(const std::string& source_file_name);

	GLuint compileSource(const GLchar* source, GLuint shader_type);

	void linkProgram();

	GLuint ShaderProgram;
	GLuint vertex_shader;
	GLuint fragment_shader;
};