#include <utils/JNI.hpp>
#include <graphics/Renderer.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeOnSurfaceChanged(JNIEnv *env, jclass,
		jint width, jint height, jboolean is_tablet)
	{
		jni::JEnv jenv(env, NULL);
		if (renderer_alive())
			renderer().on_surface_changed(width, height, is_tablet);
		(void)jenv;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeIsValid(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (renderer_alive())
			return renderer().is_valid();
		return true;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLRenderer_nativeOnDrawFrame(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (renderer_alive())
			return renderer().on_draw_frame();
	}
	
}
