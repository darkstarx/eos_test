#include <utils/JNI.hpp>
#include <Application.hpp>


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
	
	
	JNIEXPORT void JNICALL Java_com_eosproject_eos_EOSActivity_nativeOnDestroy(JNIEnv *env, jobject obj)
	{
		jni::JEnv jenv(env, obj);
		app_destroy();
		jni::clear_class_storage();
		(void)jenv;
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
	
}
