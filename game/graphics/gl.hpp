#ifndef GL_HPP
#define GL_HPP


#if defined __DESKTOP__
	#ifdef _WIN32	// Windows
		#include <glew.h>
		#include <GL/gl.h>
	#else			// Linux
		#define GL_GLEXT_PROTOTYPES
		#include <GL/gl.h>
		#include <GL/glext.h>
	#endif
#else
	#error "Unknown platform"
#endif


#ifdef __DESKTOP__
	#define GL_RGBA8_OES GL_RGBA8
#endif//__DESKTOP__


#endif//GL_HPP
