#ifndef CASTLE_GLFUNCTIONLOADER_H
#define CASTLE_GLFUNCTIONLOADER_H

#include <jni.h>
#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#if defined(_ANDROID_)
class GLFunctionLoader
{
public:
	static GLFunctionLoader & instance();

    PFNGLBINDVERTEXARRAYOESPROC bindVertexArrayOES;
    PFNGLDELETEVERTEXARRAYSOESPROC deleteVertexArraysOES;
    PFNGLGENVERTEXARRAYSOESPROC genVertexArraysOES;
    PFNGLISVERTEXARRAYOESPROC isVertexArrayOES;
	
private:
    GLFunctionLoader();
	
	

};

#endif

#endif //CASTLE_GLFUNCTIONLOADER_H