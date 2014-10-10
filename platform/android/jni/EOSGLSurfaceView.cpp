#include <utils/JNI.hpp>
#include <Application.hpp>
#include <graphics/Renderer.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeCreateRenderer(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		renderer_create();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeDestroyRenderer(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		renderer_destroy();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnTick(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		if (app_alive())
			app().tick();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnTouchEvent(JNIEnv *env, jclass,
		jint touch_action, jfloat x, jfloat y)
	{
		jni::JEnv jenv(env, NULL);
		if (app_alive())
			app().on_touch_action(touch_action_t(touch_action), graphics::position_t(x, y));
		(void)jenv;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnKeyDown(JNIEnv *env, jclass,
		jint keycode)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (app_alive())
			return app().on_key_down(keycode_t(keycode));
		return false;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnKeyUp(JNIEnv *env, jclass,
		jint keycode)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (app_alive())
			return app().on_key_up(keycode_t(keycode));
		return false;
	}
	
}
