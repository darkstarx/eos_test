#include <jni.h>
#include <Application.hpp>
#include <graphics/Renderer.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeOnSurfaceChanged(JNIEnv *env, jclass,
		jint width, jint height, jboolean is_tablet)
	{
		if (app() && app()->renderer()) {
			app()->renderer()->on_surface_changed(width, height, is_tablet);
		}
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeIsValid(JNIEnv *env, jclass)
	{
		if (app() && app()->renderer()) {
			return app()->renderer()->is_valid();
		}
		return true;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeOnDrawFrame(JNIEnv *env, jclass)
	{
		if (app() && app()->renderer()) {
			return app()->renderer()->on_draw_frame();
		}
	}
	
}
