#include <utils/JNI.hpp>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <Application.hpp>
#include <MainWindow.hpp>


extern "C"
{
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnCreate(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app_create();
		
		// Java-классы для предзагрузки
		static const char * const preload_jclasses[] = {
			"com/eosproject/eos/EOSActivity",
			"com/eosproject/resources/FileSystem"
		};
		// Кэшируем Java-классы
		for (const char * const *it(preload_jclasses), * const *end(preload_jclasses + sizeof(preload_jclasses)/sizeof(preload_jclasses[0])); it != end; ++it) {
			jclass jClass = jni::env()->FindClass(*it);
			ASSERT(jClass) << ": class = " << *it;
			jni::store_class(*it, jClass);
		}
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnStart(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app().start();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnResume(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app().resume();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnTick(JNIEnv *env, jclass)
	{
		jni::JEnv jenv(env, NULL);
		if (app_alive()) app().tick();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnPause(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app().pause();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnStop(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app().stop();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnDestroy(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app_destroy();
		jni::clear_class_storage();
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeSetSurface(JNIEnv* env, jobject obj,
		jobject surface)
	{
		jni::JEnv jenv(env, obj);
		if (surface != 0) {
			// Получаем новое окно
			EGLNativeWindowType window = ANativeWindow_fromSurface(env, surface);
			MainWindow::set_window(window);
			MainWindow::start_rendering();
		} else {
			MainWindow::stop_rendering();
			if (MainWindow::window()) {
				ANativeWindow_release(MainWindow::window());
			}
		}
		(void)jenv;
	}
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnTouchEvent(JNIEnv *env, jclass,
		jint touch_action, jfloat x, jfloat y)
	{
		jni::JEnv jenv(env, NULL);
		if (app_alive())
			app().on_touch_action(touch_action_t(touch_action), graphics::position_t(x, y));
		(void)jenv;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnKeyDown(JNIEnv *env, jclass,
		jint keycode)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (app_alive())
			return app().on_key_down(keycode_t(keycode));
		return false;
	}
	
	
	JNIEXPORT jboolean JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnKeyUp(JNIEnv *env, jclass,
		jint keycode)
	{
		jni::JEnv jenv(env, NULL);
		(void)jenv;
		if (app_alive())
			return app().on_key_up(keycode_t(keycode));
		return false;
	}
	
}
