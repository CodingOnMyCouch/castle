
//The Shader Class. This class generates the small programs that
//run on the GPU

#ifndef SHADER_H
#define SHADER_H

#include<string>

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>


class Shader
{
public: //public access specifier
		//state
	GLuint ID;

	//defualt shader constructor
	Shader() {};

	//set the current shader as active
	Shader &Use();

	//compile the shader from a given source code
	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

	//utility
	void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
	void SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
	void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void SetVector2f(const GLchar* name, const glm::vec2 &value, GLboolean useShader = false);
	void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void SetVector3f(const GLchar* name, const glm::vec3 &value, GLboolean useShader = false);
	void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void SetVector4f(const GLchar* name, const glm::vec4 &value, GLboolean useShader = false);
	void SetMatrix4(const GLchar* name, const glm::mat4 &matrix, GLboolean useShader = false);
private: //private access specifier
	void checkCompileErrors(GLuint object, std::string type);
};

#endif //SHADER_H
