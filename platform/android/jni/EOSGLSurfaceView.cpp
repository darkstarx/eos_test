#include <jni.h>
#include <Application.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeCreateRenderer(JNIEnv *env, jclass)
	{
		if (app()) {
			app()->create_renderer();
		}
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeDestroyRenderer(JNIEnv *env, jclass)
	{
		if (app()) {
			app()->destroy_renderer();
		}
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnTick(JNIEnv *env, jclass)
	{
		if (app()) {
			app()->tick();
		}
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnTouchEvent(JNIEnv *env, jclass,
		jint touch_action, jfloat x, jfloat y)
	{
		if (app()) {
			app()->on_touch_action(touch_action_t(touch_action), graphics::position_t(x, y));
		}
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnKeyDown(JNIEnv *env, jclass,
		jint keycode)
	{
		if (app()) {
			return app()->on_key_down(keycode_t(keycode));
		}
		return false;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_graphics_EOSGLSurfaceView_nativeOnKeyUp(JNIEnv *env, jclass,
		jint keycode)
	{
		if (app()) {
			return app()->on_key_up(keycode_t(keycode));
		}
		return false;
	}
	
}
