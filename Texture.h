//Texture.h

#ifndef TEXTURE_H
#define TEXTURE_H


#if !defined(_ANDROID_)
#include <GL/glew.h>
#else
#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif


//Texture is able to store and configure a texture in 2D.
//It also hosts utility functions for extra management
class Texture
{
public: //public access specifier
		//**public data variables**
		//holds the ID of the texture object
		//used for all texture operations to reference this particular texture
	GLuint ID;

	//texture image dimensions
	GLuint Width, Height; //width and height of loaded image in pixels

	GLuint Internal_Format;
	GLuint Image_Format;
	GLuint Wrap_S;
	GLuint Wrap_T;
	GLuint Filter_Min;
	GLuint Filter_Max;
	//**end of public data variables**

	//constructor
	Texture();


	void Generate(GLuint width, GLuint height, unsigned char* data);
	void Bind()const;
};



#endif //TEXTURE_H

