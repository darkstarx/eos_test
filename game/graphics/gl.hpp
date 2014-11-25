#ifndef GRAPHICS_GL_HPP
#define GRAPHICS_GL_HPP


#if defined __DESKTOP__
	#ifdef _WIN32	// Windows
		#include <glew.h>
		#include <GL/gl.h>
		#include <GL/glu.h>
	#else			// Linux
		#define GL_GLEXT_PROTOTYPES
		#include <EGL/egl.h>
		#include <GL/gl.h>
		#include <GL/glu.h>
		#include <GL/glext.h>
	#endif
#elif defined __ANDROID__
	#define GL_GLEXT_PROTOTYPES
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
#else
	#error "Unknown platform"
#endif


#ifdef __DESKTOP__
	#define GL_RGBA8_OES GL_RGBA8
#endif//__DESKTOP__


#endif //GRAPHICS_GL_HPP
