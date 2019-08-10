//ResourceManager.h

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>
#include <vector>

#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "Texture.h"
#include "Shader.h"

//a static singleton class
//this class hosts several functions to load Shaders and Textures
//each loaded Shader or Texture is stored for future reference by string handles
//all functions and resources are static and no public constructor is defined
class ResourceManager
{
public: //public access specifier
		//resource storage
	static std::map<std::string, Shader>  Shaders;
	static std::map<std::string, Texture> Textures;

	//Loads (and generates) a shader program from the file loading vertex
	//...fragment (and geometry) shader's source code. If gShaderFile is not a nullptr, 
	//..it also loads a geomtery shader
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);

	//retrieves a stored shader
	static Shader GetShader(std::string name);

	//Loads and generates a texture from a file
	static Texture LoadTexture(const GLchar* file, GLboolean alpha, std::string name);

	//retrieves a stored texture
	static Texture GetTexture(std::string name);

	static std::vector<char> LoadFile(std::string name);

	//properly de-allocates all loaded resources
	static void Clear();

private: //private access specifier
		 //private constructor to prevent any resource_manager objects from being created
		 //..all members and functions should be publically avaliable
	ResourceManager() {}

	//loads and generates a shader from file
	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);

	//Loads a single texture from file
	static Texture loadTextureFromFile(const GLchar *file, GLboolean alpha);
};


#endif //RESOURCEMANAGER_H
